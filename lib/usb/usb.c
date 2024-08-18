#include <stdio.h>
// For memcpy
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
// For led
#include "hardware/gpio.h"
// Include descriptor struct definitions

// USB register definitions from pico-sdk
#include "hardware/regs/usb.h"
// USB hardware struct definitions from pico-sdk
#include "hardware/structs/usb.h"
// For interrupt enable and numbers
#include "hardware/irq.h"
// For resetting the USB controller
#include "hardware/resets.h"

#include "usb.h"

extern struct usb_device pico;

#define usb_hw_set ((usb_hw_t *)hw_set_alias_untyped(usb_hw))
#define usb_hw_clear ((usb_hw_t *)hw_clear_alias_untyped(usb_hw))

bool is_configured(void) {
    return pico.configured;
}

/**
 * @brief Given an endpoint address, return the usb_endpoint_configuration of that endpoint. Returns NULL
 * if an endpoint of that address is not found.
 * @param addr
 * @return struct usb_endpoint*
 */
struct usb_endpoint *usb_get_endpoint_configuration(uint8_t addr) {
    struct usb_endpoint *endpoints = pico.endpoints;
    for (int i = 0; i < 3; i++) {
        if (endpoints[i].descriptor && (endpoints[i].descriptor->bEndpointAddress == addr)) {
            return &endpoints[i];
        }
    }
    return NULL;
}

/**
 * @brief Take a buffer pointer located in the USB RAM and return as an offset of the RAM.
 * @param buf
 * @return uint32_t
 */
static inline uint32_t usb_buffer_offset(volatile uint8_t *buf) {
    return (uint32_t) buf ^ (uint32_t) usb_dpram;
}

/**
 * @brief Set up the endpoint control register for an endpoint (if applicable. Not valid for EP0).
 * @param ep
 */
void usb_setup_endpoint(const struct usb_endpoint *ep) {
    // EP0 doesn't have one so return if that is the case
    if (!ep->endpoint_control) {
        return;
    }

    // Get the data buffer as an offset of the USB controller's DPRAM
    uint32_t dpram_offset = usb_buffer_offset(ep->data_buffer);
    uint32_t reg = EP_CTRL_ENABLE_BITS
                   | EP_CTRL_INTERRUPT_PER_BUFFER
                   | (ep->descriptor->bmAttributes << EP_CTRL_BUFFER_TYPE_LSB)
                   | dpram_offset;
    *ep->endpoint_control = reg;
}

/**
 * @brief Set up the endpoint control register for each endpoint.
 */
void usb_setup_endpoints(void) {
    const struct usb_endpoint *endpoints = pico.endpoints;
    for (int i = 0; i < 3; i++) {
        if (endpoints[i].descriptor && endpoints[i].handler) {
            usb_setup_endpoint(&endpoints[i]);
        }
    }
}

/**
 * @brief Set up the USB controller in device mode, clearing any previous state.
 */
void usb_device_init(void) {

    // Reset usb controller
    reset_block(RESETS_RESET_USBCTRL_BITS);
    unreset_block_wait(RESETS_RESET_USBCTRL_BITS);

    // Clear any previous state in dpram just in case
    memset(usb_dpram, 0, sizeof(*usb_dpram)); // <1>

    // Enable USB interrupt at processor
    irq_set_enabled(USBCTRL_IRQ, true);

    // Mux the controller to the onboard usb phy
    usb_hw->muxing = USB_USB_MUXING_TO_PHY_BITS | USB_USB_MUXING_SOFTCON_BITS;

    // Force VBUS detect so the device thinks it is plugged into a host
    usb_hw->pwr = USB_USB_PWR_VBUS_DETECT_BITS | USB_USB_PWR_VBUS_DETECT_OVERRIDE_EN_BITS;

    // Enable the USB controller in device mode.
    usb_hw->main_ctrl = USB_MAIN_CTRL_CONTROLLER_EN_BITS;

    // Enable an interrupt per EP0 transaction
    usb_hw->sie_ctrl = USB_SIE_CTRL_EP0_INT_1BUF_BITS; // <2>

    // Enable interrupts for when a buffer is done, when the bus is reset,
    // and when a setup packet is received
    usb_hw->inte = USB_INTS_BUFF_STATUS_BITS |
                   USB_INTS_BUS_RESET_BITS |
                   USB_INTS_SETUP_REQ_BITS;

    // Set up endpoints (endpoint control registers)
    // described by device configuration
    usb_setup_endpoints();

    // Present full speed device by enabling pull up on DP
    usb_hw_set->sie_ctrl = USB_SIE_CTRL_PULLUP_EN_BITS;
}

/**
 * @brief Given an endpoint configuration, returns true if the endpoint
 * is transmitting data to the host (i.e. is an IN endpoint)
 * @param ep, the endpoint configuration
 * @return true
 * @return false
 */
static inline bool ep_is_tx(struct usb_endpoint *ep) {
    return ep->descriptor->bEndpointAddress & USB_DIR_IN;
}

void usb_xfer(struct usb_endpoint *ep, uint8_t *buf, uint16_t len) {
    assert(len <= 64);

    // Prepare buffer control register value
    uint32_t val = len | USB_BUF_CTRL_AVAIL;

    if (ep_is_tx(ep)) {
        // Need to copy the data from the user buffer to the usb memory
        memcpy((void*)ep->data_buffer, (void*)buf, len);
        // Mark as full
        val |= USB_BUF_CTRL_FULL;
    }

    // Set pid and flip for next transfer
    if (ep->next_pid == 0) {
        val |= USB_BUF_CTRL_DATA0_PID;
        ep->next_pid = 1;
    }
    else {
        val |= USB_BUF_CTRL_DATA1_PID;
        ep->next_pid = 0;
    }

    val |= USB_BUF_CTRL_LAST;

    *ep->buffer_control = val & ~USB_BUF_CTRL_AVAIL;

    busy_wait_at_least_cycles(12);

    *ep->buffer_control = val;
}

/**
 * @param ep
 * @param buf
 * @param len
 */
void usb_great_xfer(struct usb_endpoint *ep, uint8_t *buf, uint16_t len) {
    // size_t remaining_len = len;
    // uint16_t ep_buf_size = ep->descriptor->wMaxPacketSize;

    // do {
    //     size_t pkt_len = (remaining_len > ep_buf_size) ? ep_buf_size : remaining_len;

    //     // Prepare buffer control register value
    //     uint32_t val = len | USB_BUF_CTRL_AVAIL;

    //     if (ep_is_tx(ep)) {
    //         // Need to copy the data from the user buffer to the usb memory
    //         memcpy((void *)ep->data_buffer, (void*)buf, pkt_len);
    //         // Mark as full
    //         val |= USB_BUF_CTRL_FULL;
    //     }

    //     // Set pid and flip for next transfer
    //     if (ep->next_pid == 0) {
    //         val |= USB_BUF_CTRL_DATA0_PID;
    //         ep->next_pid = 1;
    //     }
    //     else {
    //         val |= USB_BUF_CTRL_DATA1_PID;
    //         ep->next_pid = 0;
    //     }

    //     val |= USB_BUF_CTRL_LAST;

    //     *ep->buffer_control = val & ~USB_BUF_CTRL_AVAIL;

    //     busy_wait_at_least_cycles(12);

    //     *ep->buffer_control = val;

    //     buf += pkt_len;
    //     remaining_len -= pkt_len;
    // }
    // while (remaining_len > 0);

    uint16_t remaining_len = len;
    size_t ep_buf_size = ep->descriptor->wMaxPacketSize;

    while (remaining_len > 0) {
        size_t pkt_size = (remaining_len > ep_buf_size) ? ep_buf_size : remaining_len;

        usb_xfer(ep, buf, pkt_size);

        while (*ep->buffer_control & USB_BUF_CTRL_FULL) {
            tight_loop_contents();
        }

        buf += pkt_size;
        remaining_len -= pkt_size;
    }
}

void usb_control_xfer(uint8_t *buf, uint16_t len) {
    usb_xfer(usb_get_endpoint_configuration(EP0_OUT_ADDR), buf, len);
}

void usb_xfer_ep0_in(uint8_t *buf, uint16_t len) {
    struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_IN_ADDR);
    usb_xfer(ep, buf, len);
}

/**
 * @brief Handle a BUS RESET from the host by setting the device address back to 0.
 */
void usb_bus_reset(void) {
    // Set address back to 0
    pico.addr = 0;
    pico.should_set_addr = false;
    usb_hw->dev_addr_ctrl = 0;
    pico.configured = false;
}

/**
 * @brief Sends a zero length status packet back to the host.
 */
void usb_acknowledge_out_request(void) {
    usb_xfer(usb_get_endpoint_configuration(EP0_IN_ADDR), NULL, 0);
}

/**
 * @brief Handles a SET_ADDR request from the host. The actual setting of the device address in
 * hardware is done in ep0_in_handler. This is because we have to acknowledge the request first
 * as a device with address zero.
 * @param pkt, the setup packet from the host.
 */
void usb_set_device_address(volatile struct usb_setup_packet *pkt) {
    // Set address is a bit of a strange case because we have to send a 0 length status packet first with
    // address 0
    pico.addr = (pkt->wValue & 0xff);
    // Will set address in the callback phase
    pico.should_set_addr = true;
    usb_acknowledge_out_request();
}

/**
 * @brief Handles a SET_CONFIGRUATION request from the host. Assumes one configuration so simply
 * sends a zero length status packet back to the host.
 * @param pkt, the setup packet from the host.
 */
void usb_set_device_configuration(volatile struct usb_setup_packet *pkt) {
    // Only one configuration so just acknowledge the request
    usb_acknowledge_out_request();
    pico.configured = true;
}

/**
 * @brief Respond to a setup packet from the host.
 */
void usb_handle_setup_packet(void) {
    volatile struct usb_setup_packet *pkt = (volatile struct usb_setup_packet *) &usb_dpram->setup_packet;
    uint8_t req_direction = pkt->bmRequestType;
    uint8_t req = pkt->bRequest;

    // Reset PID to 1 for EP0 IN
    usb_get_endpoint_configuration(EP0_IN_ADDR)->next_pid = 1u;

    if (req_direction == USB_DIR_OUT) {
        if (req == USB_REQUEST_SET_ADDRESS) {
            usb_set_device_address(pkt);
        }
        else if (req == USB_REQUEST_SET_CONFIGURATION) {
            usb_set_device_configuration(pkt);
        }
        else {
            usb_acknowledge_out_request();
        }
    }
    else if (req_direction == USB_DIR_IN) {
        if (req == USB_REQUEST_GET_DESCRIPTOR) {
            uint16_t descriptor_type = pkt->wValue >> 8;

            switch (descriptor_type) {
                case USB_DESCRIPTOR_TYPE_DEVICE:
                    usb_handle_device_descriptor(pkt->wLength);
                    break;

                case USB_DESCRIPTOR_TYPE_CONFIG:
                    usb_handle_config_descriptor(pkt->wLength);
                    break;

                case USB_DESCRIPTOR_TYPE_STRING:
                    usb_handle_string_descriptor(pkt->wValue & 0xFF, pkt->wIndex);
                    break;

                default:
            }
        } else {

        }
    }
    else if (req_direction == 0x21) {
        if (req == USB_REQUEST_SET_IDLE) {
            //uint8_t value = pkt->wValue;
            usb_acknowledge_out_request();

        }
        else if (req == USB_REQUEST_SET_REPORT) {
            usb_acknowledge_out_request();
            static struct usb_keyboard_report rp = {.id = 0x01};
            usb_send_keyboard_report(&rp);
        }
    }
    else if (req_direction == EP_IN_HID) {
        if (req == USB_REQUEST_GET_DESCRIPTOR) {
            uint16_t descriptor_type = pkt->wValue >> 8;

            switch (descriptor_type) {
                case USB_DESCRIPTOR_TYPE_REPORT:
                    usb_handle_report_descriptor(pkt->wLength);
                    static struct usb_keyboard_report rp = {.id = 0x01};
                    usb_send_keyboard_report(&rp);
                    break;
            }
        }
    }
}

/**
 * @brief Notify an endpoint that a transfer has completed.
 * @param ep, the endpoint to notify.
 */
static void usb_handle_ep_buff_done(struct usb_endpoint *ep) {
    uint32_t buffer_control = *ep->buffer_control;
    // Get the transfer length for this endpoint
    uint16_t len = buffer_control & USB_BUF_CTRL_LEN_MASK;

    // Call that endpoints buffer done handler
    ep->handler((uint8_t*)ep->data_buffer, len);
}

/**
 * @brief Find the endpoint configuration for a specified endpoint number and
 * direction and notify it that a transfer has completed.
 * @param ep_num
 * @param in
 */
static void usb_handle_buff_done(uint ep_num, bool in) {
    uint8_t ep_addr = ep_num | (in ? USB_DIR_IN : 0);

    for (uint i = 0; i < 3; i++) {
        struct usb_endpoint *ep = &pico.endpoints[i];
        if (ep->descriptor && ep->handler) {
            if (ep->descriptor->bEndpointAddress == ep_addr) {
                usb_handle_ep_buff_done(ep);
                return;
            }
        }
    }
}

/**
 * @brief Handle a "buffer status" irq. This means that one or more
 * buffers have been sent / received. Notify each endpoint where this
 * is the case.
 */
static void usb_handle_buff_status(void) {
    uint32_t buffers = usb_hw->buf_status;
    uint32_t remaining_buffers = buffers;

    uint bit = 1u;
    for (uint i = 0; remaining_buffers && i < 3 * 2; i++) {
        if (remaining_buffers & bit) {
            // clear this in advance
            usb_hw_clear->buf_status = bit;
            // IN transfer for even i, OUT transfer for odd i
            usb_handle_buff_done(i >> 1u, !(i & 1u));
            remaining_buffers &= ~bit;
        }
        bit <<= 1u;
    }
}

void usb_handle_suspend(void) {
    pico.suspended = true;
    return;
}

void usb_handle_resume(void) {
    pico.suspended = false;
    return;
}

/**
 * @brief USB interrupt handler
 */
void isr_usbctrl(void) {
    // USB interrupt handler
    uint32_t status = usb_hw->ints;
    uint32_t handled = 0;

    // Setup packet received
    if (status & USB_INTS_SETUP_REQ_BITS) {
        handled |= USB_INTS_SETUP_REQ_BITS;
        usb_hw_clear->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
        usb_handle_setup_packet();
    }

    // Buffer status, one or more buffers have completed
    if (status & USB_INTS_BUFF_STATUS_BITS) {
        handled |= USB_INTS_BUFF_STATUS_BITS;
        usb_handle_buff_status();
    }

    // Bus is reset
    if (status & USB_INTS_BUS_RESET_BITS) {
        handled |= USB_INTS_BUS_RESET_BITS;
        usb_hw_clear->sie_status = USB_SIE_STATUS_BUS_RESET_BITS;
        usb_bus_reset();
    }

    // Device is suspended
    if (status & USB_INTS_DEV_SUSPEND_BITS) {
        usb_handle_suspend();
        usb_hw_clear->sie_status = USB_SIE_STATUS_SUSPENDED_BITS;
    }

    // Device is resumed
    if (status & USB_INTS_DEV_RESUME_FROM_HOST_BITS) {
        usb_handle_resume();
        usb_hw_clear->sie_status = USB_SIE_STATUS_RESUME_BITS;
    }


    if (status ^ handled) {
        panic("Unhandled IRQ 0x%x\n", (uint) (status ^ handled));
    }
}

void set_report_callback(uint8_t const *buf, uint16_t len) {

}
