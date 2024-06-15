/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
// For memcpy
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
// For led 
#include "hardware/gpio.h"
// Include descriptor struct definitions
#include "usb_common.h"
// USB register definitions from pico-sdk
#include "hardware/regs/usb.h"
// USB hardware struct definitions from pico-sdk
#include "hardware/structs/usb.h"
// For interrupt enable and numbers
#include "hardware/irq.h"
// For resetting the USB controller
#include "hardware/resets.h"

#include "usb.h"

#define usb_hw_set ((usb_hw_t *)hw_set_alias_untyped(usb_hw))
#define usb_hw_clear ((usb_hw_t *)hw_clear_alias_untyped(usb_hw))

// Global device address
static bool should_set_address = false;
static uint8_t dev_addr = 0;
static volatile bool configured = false;

/**
 * @author Styann
 * @return boolean
 */
bool is_configured(void) {
    return configured;
}

// Global data buffer for EP0
static uint8_t ep0_buf[EP0_BUF_SIZE];

// Struct defining the device configuration
static struct usb_device_configuration dev_config = {
        .suspended = false,
        .configured = false,
        .device_descriptor = &device_descriptor,
        .interface_descriptor = &hid_interface_descriptor,
        .config_descriptor = &config_descriptor,
        .hid_descriptor = &hid_descriptor,
        .report_descriptor = report_descriptor,
        .string_descriptors = { &lang_descriptor, &vendor_descriptor, &product_descriptor },
        .endpoints = {
                {
                    .descriptor = &ep0_out,
                    .handler = &ep0_out_handler,
                    .endpoint_control = NULL, // NA for EP0
                    .buffer_control = &usb_dpram->ep_buf_ctrl[0].out,
                    // EP0 in and out share a data buffer
                    .data_buffer = &usb_dpram->ep0_buf_a[0],
                },
                {
                    .descriptor = &ep0_in,
                    .handler = &ep0_in_handler,
                    .endpoint_control = NULL, // NA for EP0,
                    .buffer_control = &usb_dpram->ep_buf_ctrl[0].in,
                    // EP0 in and out share a data buffer
                    .data_buffer = &usb_dpram->ep0_buf_a[0],
                },
                {
                    .descriptor = &ep1_in_hid,
                    .handler = &ep1_in_hid_handler,
                    .endpoint_control = &usb_dpram->ep_ctrl[0].in,
                    .buffer_control = &usb_dpram->ep_buf_ctrl[1].in,
                    .data_buffer = &usb_dpram->epx_data[0 * EP1_BUF_SIZE]
                }
        }
};

/**
 * @brief Given an endpoint address, return the usb_endpoint_configuration of that endpoint. Returns NULL
 * if an endpoint of that address is not found.
 *
 * @param addr
 * @return struct usb_endpoint*
 */
struct usb_endpoint *usb_get_endpoint_configuration(uint8_t addr) {
    struct usb_endpoint *endpoints = dev_config.endpoints;
    for (int i = 0; i < USB_NUM_ENDPOINTS; i++) {
        if (endpoints[i].descriptor && (endpoints[i].descriptor->bEndpointAddress == addr)) {
            return &endpoints[i];
        }
    }
    return NULL;
}

/**
 * @brief Take a buffer pointer located in the USB RAM and return as an offset of the RAM.
 *
 * @param buf
 * @return uint32_t
 */
static inline uint32_t usb_buffer_offset(volatile uint8_t *buf) {
    return (uint32_t) buf ^ (uint32_t) usb_dpram;
}

/**
 * @brief Set up the endpoint control register for an endpoint (if applicable. Not valid for EP0).
 *
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
 *
 */
void usb_setup_endpoints(void) {
    const struct usb_endpoint *endpoints = dev_config.endpoints;
    for (int i = 0; i < USB_NUM_ENDPOINTS; i++) {
        if (endpoints[i].descriptor && endpoints[i].handler) {
            usb_setup_endpoint(&endpoints[i]);
        }
    }
}

/**
 * @brief Set up the USB controller in device mode, clearing any previous state.
 *
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
 *
 * @param ep, the endpoint configuration
 * @return true
 * @return false
 */
static inline bool ep_is_tx(struct usb_endpoint *ep) {
    return ep->descriptor->bEndpointAddress & USB_DIR_IN;
}

/**
 * @brief Starts a transfer on a given endpoint.
 *
 * @param ep, the endpoint configuration.
 * @param buf, the data buffer to send. Only applicable if the endpoint is TX
 * @param len, the length of the data in buf (this example limits max len to one packet - 64 bytes)
 */
void usb_start_transfer(struct usb_endpoint *ep, uint8_t *buf, uint16_t len) {
    // We are asserting that the length is <= 64 bytes for simplicity of the example.
    // For multi packet transfers see the tinyusb port.
    assert(len <= 64);

    printf("Start transfer of len %d on ep addr 0x%x\n", len, ep->descriptor->bEndpointAddress);

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
 * @author Styann
 * @brief Sends a packet larger than the buffer size in several smaller packets
 */
void usb_start_great_transfer(struct usb_endpoint *ep, uint8_t *buf, uint16_t len){

	/*uint8_t chunksize = ep->descriptor->wMaxPacketSize;
	uint8_t remainder_size = len % chunksize;

	for (uint8_t offset = 0; offset < (len - remainder_size); offset += chunksize) {

        usb_start_xfer(ep, buf + offset, chunksize, false);

	}

	if (remainder_size > 0) {
        usb_start_xfer(ep, buf + (len - remainder_size), remainder_size, true);
        //usb_acknowledge_out_request();
	}*/
    assert(ep_is_tx(ep));

    uint16_t wMaxPacketSize = ep->descriptor->wMaxPacketSize;
    uint16_t transferred_len = 0;
    uint16_t remaining_len = len;
    uint16_t offset = 0;

    while (transferred_len < len) {
        uint16_t pkt_len = MIN(remaining_len, wMaxPacketSize);
        // Prepare buffer control register value
        uint32_t val = pkt_len | USB_BUF_CTRL_AVAIL;

        // Need to copy the data from the user buffer to the usb memory
        memcpy((void*)ep->data_buffer, (void*)(buf + transferred_len), pkt_len);
        // Mark as full
        val |= USB_BUF_CTRL_FULL;

        // Set pid and flip for next transfer
        if (ep->next_pid == 0) {
            val |= USB_BUF_CTRL_DATA0_PID;
            ep->next_pid = 1;
        }
        else {
            val |= USB_BUF_CTRL_DATA1_PID;
            ep->next_pid = 0;
        }

        if (remaining_len <= wMaxPacketSize) {
            val |= USB_BUF_CTRL_LAST;
        }

        *ep->buffer_control = val & ~USB_BUF_CTRL_AVAIL;

        busy_wait_at_least_cycles(12);

        *ep->buffer_control = val;

        transferred_len += pkt_len;
        remaining_len -= pkt_len;
    } 
}

/**
 * @brief Send device descriptor to host
 */
void usb_handle_device_descriptor(volatile struct usb_setup_packet *pkt) {
    const struct usb_device_descriptor *data = dev_config.device_descriptor;
    // EP0 in
    struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_IN_ADDR);
    // Always respond with pid 1
    ep->next_pid = 1;
    usb_start_transfer(ep, (uint8_t*)data, MIN(sizeof(struct usb_device_descriptor), pkt->wLength));
}

/**
 * @author Styann
 * @brief Send report descriptor to host
 */
void usb_handle_report_descriptor(volatile struct usb_setup_packet *pkt){
    struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_IN_ADDR);
    usb_start_transfer(ep, (uint8_t*)report_descriptor, 64);
    usb_start_transfer(ep, (uint8_t*)(report_descriptor + 64), 28);
    // usb_start_transfer(ep, (uint8_t*)fightstick_report_descriptor, USB_FIGHTSTICK_REPORT_DESCRIPTOR_LENGTH);
}

/**
 * @brief Send the configuration descriptor (and potentially the configuration and endpoint descriptors) to the host.
 * @param pkt, the setup packet received from the host.
 */
void usb_handle_config_descriptor(volatile struct usb_setup_packet *pkt) {
    uint8_t *buf = &ep0_buf[0];

    // First request will want just the config descriptor
    const struct usb_configuration_descriptor *d = dev_config.config_descriptor;
    memcpy((void *) buf, d, sizeof(struct usb_configuration_descriptor));
    buf += sizeof(struct usb_configuration_descriptor);

    // If we more than just the config descriptor copy it all
    if (pkt->wLength >= d->wTotalLength) {
        memcpy((void *) buf, dev_config.interface_descriptor, sizeof(struct usb_interface_descriptor));
        buf += sizeof(struct usb_interface_descriptor);

        memcpy((void *) buf, dev_config.hid_descriptor, sizeof(struct usb_hid_descriptor));
        buf += sizeof(struct usb_hid_descriptor);

        const struct usb_endpoint *ep = dev_config.endpoints;

        // Copy all the endpoint descriptors starting from EP1
        for (uint i = 2; i < USB_NUM_ENDPOINTS; i++) {
            if (ep[i].descriptor) {
                memcpy((void *) buf, ep[i].descriptor, sizeof(struct usb_endpoint_descriptor));
                buf += sizeof(struct usb_endpoint_descriptor);
            }
        }

    }

    // Send data
    // Get len by working out end of buffer subtract start of buffer
    uint32_t len = (uint32_t) buf - (uint32_t) &ep0_buf[0];
    usb_start_transfer(usb_get_endpoint_configuration(EP0_IN_ADDR), &ep0_buf[0], MIN(len, pkt->wLength));
}

/**
 * @brief Handle a BUS RESET from the host by setting the device address back to 0.
 */
void usb_bus_reset(void) {
    // Set address back to 0
    dev_addr = 0;
    should_set_address = false;
    usb_hw->dev_addr_ctrl = 0;
    configured = false;
    dev_config.configured = false;
}

/**
 * @brief Send the requested string descriptor to the host.
 * @param pkt, the setup packet from the host.
 */
void usb_handle_string_descriptor(volatile struct usb_setup_packet *pkt) {
    uint8_t descriptorIndex = pkt->wValue;

    struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_IN_ADDR);
    const struct usb_string_descriptor *string_descriptor = dev_config.string_descriptors[descriptorIndex];

    if (descriptorIndex == 0) {
        usb_start_transfer(ep, (uint8_t*)string_descriptor, string_descriptor->bLength);
    }
    else {
        uint8_t *buf = calloc(string_descriptor->bLength, 1);

        *buf = string_descriptor->bLength;
        *(buf + 1) = string_descriptor->bDescriptorType;

        for (uint16_t i = 0; i < (string_descriptor->bLength / 2); i++) {
            uint16_t buf_offset = i + i + 2;
            *(buf + buf_offset) = (uint8_t)*(string_descriptor->bString + i);
        }

        usb_start_transfer(ep, (uint8_t*)buf, string_descriptor->bLength);
        free(buf);
    }

    return;
}

/**
 * @brief Sends a zero length status packet back to the host.
 */
void usb_acknowledge_out_request(void) {
    usb_start_transfer(usb_get_endpoint_configuration(EP0_IN_ADDR), NULL, 0);
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
    dev_addr = (pkt->wValue & 0xff);
    //("Set address %d\r\n", dev_addr);
    // Will set address in the callback phase
    should_set_address = true;
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
    configured = true;
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
                    usb_handle_device_descriptor(pkt);
                    break;

                case USB_DESCRIPTOR_TYPE_CONFIG:
                    usb_handle_config_descriptor(pkt);
                    break;

                case USB_DESCRIPTOR_TYPE_STRING:
                    usb_handle_string_descriptor(pkt);
                    break;

                default:
            }
        } else {

        }
    }
    else if(req_direction == 0x21) {
        if(req == USB_HID_REQUEST_SET_IDLE) {
            //uint8_t value = pkt->wValue;
            usb_acknowledge_out_request();
        }
        else if(req == USB_HID_REQUEST_SET_REPORT) {
            // TODO: fix this
            struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_OUT_ADDR);
            uint8_t const *buf = (uint8_t const*)ep->data_buffer;

            set_report_callback(buf, pkt->wLength);

            usb_acknowledge_out_request();
        }
    }
    else if(req_direction == EP_IN_HID) {
        if(req == USB_REQUEST_GET_DESCRIPTOR) {
            uint16_t descriptor_type = pkt->wValue >> 8;

            switch(descriptor_type) {
                case USB_HID_DESCRIPTOR_TYPE_REPORT:
                    usb_handle_report_descriptor(pkt);
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

    for (uint i = 0; i < USB_NUM_ENDPOINTS; i++) {
        struct usb_endpoint *ep = &dev_config.endpoints[i];
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
    for (uint i = 0; remaining_buffers && i < USB_NUM_ENDPOINTS * 2; i++) {
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
    dev_config.suspended = true;
    return;
}

void usb_handle_resume(void) {
    dev_config.suspended = false;
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

/**
 * @brief EP0 in transfer complete. Either finish the SET_ADDRESS process, or receive a zero
 * length status packet from the host.
 * @param buf the data that was sent
 * @param len the length that was sent
 */
void ep0_in_handler(uint8_t *buf, uint16_t len) {
    if (should_set_address) {
        // Set actual device address in hardware
        usb_hw->dev_addr_ctrl = dev_addr;
        should_set_address = false;
    } else {
        // Receive a zero length status packet from the host on EP0 OUT
        struct usb_endpoint *ep = usb_get_endpoint_configuration(EP0_OUT_ADDR);
        usb_start_transfer(ep, NULL, 0);
    }
}

void ep0_out_handler(uint8_t *buf, uint16_t len) {

}

void ep1_in_hid_handler(uint8_t *buf, uint16_t len) {

}

void set_report_callback(uint8_t const *buf, uint16_t len) {

}
