#include "usb.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/regs/usb.h" // USB register definitions from pico-sdk
#include "hardware/structs/usb.h" // USB hardware struct definitions from pico-sdk
#include "hardware/irq.h" // for interrupt enable and numbers
#include "hardware/resets.h" // for resetting the USB controller

#include "hardware/uart.h"

static const struct usb_device_descriptor device_descriptor = {
    .bLength            = sizeof(struct usb_device_descriptor),
    .bDescriptorType    = USB_DESCRIPTOR_TYPE_DEVICE,
    .bcdUSB             = 0x0110, // USB 1.1 device
    .bDeviceClass       = 0,
    .bDeviceSubClass    = 0,
    .bDeviceProtocol    = 0,
    .bMaxPacketSize0    = 64,
    .idVendor           = 0x2E8A,
    .idProduct          = 0x0003,
    .bcdDevice          = 0x0100,
    .iManufacturer      = 1,
    .iProduct           = 2,
    .iSerialNumber      = 0,
    .bNumConfigurations = 1
};

static const struct usb_configuration_descriptor config_descriptor = {
    .bLength             = sizeof(struct usb_configuration_descriptor),
    .bDescriptorType     = USB_DESCRIPTOR_TYPE_CONFIG,
    .wTotalLength        = sizeof(struct usb_configuration_descriptor) + (
                                (sizeof(struct usb_interface_descriptor)
                                + sizeof(struct usb_hid_descriptor)
                                + sizeof(struct usb_endpoint_descriptor)) * 2
                            ),
    .bNumInterfaces      = 1,
    .bConfigurationValue = 1, // Configuration 1
    .iConfiguration      = 0, // No string
    .bmAttributes        = 0xA0, // self powered, remote wakeup
    .bMaxPower           = 50  // 100ma
};

static const struct usb_interface_descriptor keyboard_interface_descriptor = {
    .bLength            = sizeof(struct usb_interface_descriptor),
    .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber   = 0,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 0x01, // Interface has 1 endpoints
    .bInterfaceClass    = USB_INTERFACE_CLASS_TYPE_HID,
    .bInterfaceSubClass = USB_INTERFACE_SUBCLASS_TYPE_BOOT_INTERFACE,
    .bInterfaceProtocol = USB_INTERFACE_PROTOCOL_KEYBOARD,
    .iInterface         = 0
};

static const struct usb_interface_descriptor mouse_interface_descriptor = {
    .bLength            = sizeof(struct usb_interface_descriptor),
    .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
    .bInterfaceNumber   = 1,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 0x01, // Interface has 1 endpoints
    .bInterfaceClass    = USB_INTERFACE_CLASS_TYPE_HID, // Vendor specific endpoint
    .bInterfaceSubClass = USB_INTERFACE_SUBCLASS_TYPE_BOOT_INTERFACE,
    .bInterfaceProtocol = USB_INTERFACE_PROTOCOL_MOUSE,
    .iInterface         = 0
};

static const struct usb_endpoint_descriptor ep0_out_descriptor = {
    .bLength          = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress = 0x00, // EP number 0, OUT from host (rx to device)
    .bmAttributes     = USB_TRANSFER_TYPE_CONTROL,
    .wMaxPacketSize   = 64,
    .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep0_in_descriptor = {
    .bLength          = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress = 0x80, // EP number 0, OUT from host (rx to device)
    .bmAttributes     = USB_TRANSFER_TYPE_CONTROL,
    .wMaxPacketSize   = 64,
    .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep1_in_descriptor = {
    .bLength          = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress = 0x81,
    .bmAttributes     = USB_TRANSFER_TYPE_INTERRUPT,
    .wMaxPacketSize   = 64,
    .bInterval        = 5 // ms
};

static const struct usb_endpoint_descriptor ep2_in_descriptor = {
    .bLength          = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress = 0x82,
    .bmAttributes     = USB_TRANSFER_TYPE_INTERRUPT,
    .wMaxPacketSize   = 64,
    .bInterval        = 5 // ms
};

static const struct usb_endpoint_descriptor ep_control_descriptor = {
    .bLength = sizeof(struct usb_endpoint_descriptor),
    .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
    .bEndpointAddress = 0x40,
    .bmAttributes     = USB_TRANSFER_TYPE_CONTROL,
    .wMaxPacketSize   = 64,
    .bInterval        = 0
};

static const struct usb_string_language_descriptor language_descriptor = {
    .bLength = sizeof(struct usb_string_language_descriptor),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .wLANGID = USB_LANGUAGE_ID_FRENCH
};

static const uint8_t keyboard_report_descriptor[] = {
    USAGE_PAGE, GENERIC_DESKTOP_PAGE,

    USAGE, USAGE_KEYBOARD,
    COLLECTION, COLLECTION_APPLICATION,
            REPORT_ID, 0x01,
            // led
            USAGE_PAGE, LED_PAGE,
            USAGE_MINIMUM, 0x01,
            USAGE_MAXIMUM, 0x03,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM, 0x01,
            REPORT_SIZE, 0x01,
            REPORT_COUNT, 0x03,
            OUTPUT, DATA_VAR_ABS,
            REPORT_COUNT, 0x05,
            OUTPUT, CONST_VAR_ABS,
            // keyboard
            USAGE_PAGE, KEYBOARD_KEYPAD_PAGE,
            // modifier field
            USAGE_MINIMUM, KC_CTRL_LEFT,
            USAGE_MAXIMUM, KC_GUI_RIGHT,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM, 0x01,
            REPORT_SIZE, 0x01,
            REPORT_COUNT, 0x08,
            INPUT, DATA_VAR_ABS,
            // reserved field
            REPORT_COUNT, 0x01,
            REPORT_SIZE, 0x08,
            INPUT, CONST_ARRAY_ABS,
            REPORT_COUNT, 0x05,
            REPORT_SIZE, 0x01,
            // keycodes
            REPORT_COUNT, 0x06,
            REPORT_SIZE, 0x08,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM, 0x65,
            USAGE_PAGE, KEYBOARD_KEYPAD_PAGE,
            USAGE_MINIMUM, 0x00,
            USAGE_MAXIMUM, 0xFF,
            INPUT, DATA_ARRAY_ABS,
    END_COLLECTION,

    USAGE_PAGE, CONSUMER_PAGE,
    USAGE, USAGE_CONSUMER_CONTROL,
    COLLECTION, COLLECTION_APPLICATION,
            REPORT_ID, 0x02,
            LOGICAL_MINIMUM, 0x00,
            LOGICAL_MAXIMUM + 1, 0xFF, 0x03,
            USAGE_MINIMUM, 0x00,
            0x2A, 0xFF, 0x03,
            REPORT_COUNT, 1,
            REPORT_SIZE, 16,
            INPUT, DATA_ARRAY_ABS,
    END_COLLECTION
};

static const uint8_t mouse_report_descriptor[] = {
    USAGE_PAGE, GENERIC_DESKTOP_PAGE,

    USAGE, USAGE_MOUSE,
    COLLECTION, COLLECTION_APPLICATION,
        REPORT_ID, 0x03,
        USAGE, USAGE_POINTER,
        COLLECTION, COLLECTION_PHYSICAL,
            USAGE, USAGE_BUTTON,
            USAGE_MINIMUM, 0x01,
            USAGE_MINIMUM, 0x05,
            LOGICAL_MINIMUM, 0,
            LOGICAL_MAXIMUM, 1,
            REPORT_COUNT, 5,
            REPORT_SIZE, 1,
            INPUT, DATA_VAR_ABS,

            USAGE_PAGE, GENERIC_DESKTOP_PAGE,
            USAGE, USAGE_X,
            USAGE, USAGE_Y,
            LOGICAL_MINIMUM + 1, 0x00, 0x80,
            LOGICAL_MAXIMUM + 1, 0xFF, 0x7F,
            REPORT_SIZE, 16,
            REPORT_COUNT, 2,
            INPUT, DATA_VAR_REL,

            USAGE_PAGE, GENERIC_DESKTOP_PAGE,
            USAGE, USAGE_WHEEL,
            LOGICAL_MINIMUM, -127,
            LOGICAL_MAXIMUM, 127,
            REPORT_SIZE, 8,
            REPORT_COUNT, 1,
            INPUT, DATA_VAR_REL,
        END_COLLECTION,
    END_COLLECTION
};

static const struct usb_hid_descriptor keyboard_hid_descriptor = {
    .bLength         = sizeof(struct usb_hid_descriptor),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_HID,
    .bcdHID          = 0x0111,
    .bCountryCode    = USB_COUNTRY_CODE_FRENCH,
    .bNumDescriptors = 0x01,
    .bReportType     = USB_DESCRIPTOR_TYPE_REPORT,
    .wReportLength   = sizeof(keyboard_report_descriptor)
};

static const struct usb_hid_descriptor mouse_hid_descriptor = {
    .bLength         = sizeof(struct usb_hid_descriptor),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_HID,
    .bcdHID          = 0x0111,
    .bCountryCode    = USB_COUNTRY_CODE_FRENCH,
    .bNumDescriptors = 0x01,
    .bReportType     = USB_DESCRIPTOR_TYPE_REPORT,
    .wReportLength   = sizeof(mouse_report_descriptor)
};

usb_device_t pico = {
    .addr = 0x00,
    .should_set_addr = false,

    .suspended = false,
    .configured = false,

    .device_descriptor = &device_descriptor,

    .configuration_descriptor = &config_descriptor,
    .hid_interfaces = {
        {
            .interface_descriptor = &keyboard_interface_descriptor,
            .hid_descriptor = &keyboard_hid_descriptor,
            .endpoint = {
                .descriptor = &ep1_in_descriptor,
                .handler = &ep1_in_handler,
                .endpoint_control = &usb_dpram->ep_ctrl[0].in,
                .buffer_control = &usb_dpram->ep_buf_ctrl[1].in,
                .data_buffer = &usb_dpram->epx_data[0 * 64]
            }
        },
        {
            .interface_descriptor = &mouse_interface_descriptor,
            .hid_descriptor = &mouse_hid_descriptor,
            .endpoint = {
                .descriptor = &ep2_in_descriptor,
                .handler = &ep2_in_handler,
                .endpoint_control = &usb_dpram->ep_ctrl[1].in,
                .buffer_control = &usb_dpram->ep_buf_ctrl[2].in,
                .data_buffer = &usb_dpram->epx_data[1 * 64]
            }
        }
    },

    .language_descriptor = &language_descriptor,
    .vendor = "Seegson Electronics",
    .product = "Arquebus Keyboard",

    .ep0_in = {
            .descriptor = &ep0_in_descriptor,
            .handler = &ep0_in_handler,
            .endpoint_control = NULL, // NA for EP0,
            .buffer_control = &usb_dpram->ep_buf_ctrl[0].in,
            // EP0 in and out share a data buffer
            .data_buffer = &usb_dpram->ep0_buf_a[0],
    },
    .ep0_out = {
        .descriptor = &ep0_out_descriptor,
        .handler = &ep0_out_handler,
        .endpoint_control = NULL, // NA for EP0
        .buffer_control = &usb_dpram->ep_buf_ctrl[0].out,
        // EP0 in and out share a data buffer
        .data_buffer = &usb_dpram->ep0_buf_a[0],
    }
};

#define usb_hw_set ((usb_hw_t *)hw_set_alias_untyped(usb_hw))
#define usb_hw_clear ((usb_hw_t *)hw_clear_alias_untyped(usb_hw))

/**
 * @brief Given an endpoint address, return the usb_endpoint_configuration of that endpoint. Returns NULL
 * if an endpoint of that address is not found.
 * @param addr
 * @return struct usb_endpoint*
 */
// struct usb_endpoint *usb_get_endpoint_configuration(uint8_t addr) {
//     struct usb_endpoint *endpoints = pico.endpoints;
//
//     for (int i = 0; i < 3; i++) {
//         if (endpoints[i].descriptor && (endpoints[i].descriptor->bEndpointAddress == addr)) {
//             return &endpoints[i];
//         }
//     }
//     return NULL;
// }

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
void usb_setup_endpoint(struct usb_endpoint *ep) {
    // EP0 doesn't have one so return if that is the case
    if (!ep->endpoint_control) return;

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
void usb_setup_endpoints(usb_device_t *const device) {
    usb_setup_endpoint(&(device->ep0_in));
    usb_setup_endpoint(&(device->ep0_out));

    for (int i = 0; i < 2; i++) {
        struct usb_endpoint *ep = &(device->hid_interfaces[i].endpoint);

        if (ep->descriptor && ep->handler) {
            usb_setup_endpoint(ep);
        }
    }

    // usb_setup_endpoint(&pico.ep0_in);
    // usb_setup_endpoint(&pico.ep0_out);

    // for (int i = 0; i < 2; i++) {
    //     struct usb_endpoint *ep = &pico.hid_interfaces[i].endpoint;

    //     if (ep->descriptor && ep->handler) {
    //         usb_setup_endpoint(ep);
    //     }
    // }
}

/**
 * @brief set up the USB controller in device mode, clearing any previous state.
 */
void usb_device_init(void) {
    // reset usb controller
    reset_block(RESETS_RESET_USBCTRL_BITS);
    unreset_block_wait(RESETS_RESET_USBCTRL_BITS);

    // clear dpram
    memset(usb_dpram, 0, sizeof(*usb_dpram));

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
    usb_hw->inte = USB_INTS_BUFF_STATUS_BITS
        | USB_INTS_BUS_RESET_BITS
        | USB_INTS_SETUP_REQ_BITS
        | USB_INTS_DEV_SUSPEND_BITS
        | USB_INTS_DEV_RESUME_FROM_HOST_BITS;

    // Set up endpoints (endpoint control registers)
    // described by device configuration
    usb_setup_endpoints(&pico);

    // Present full speed device by enabling pull up on DP
    usb_hw_set->sie_ctrl = USB_SIE_CTRL_PULLUP_EN_BITS;

    // enable USB interrupt at processor
    irq_set_enabled(USBCTRL_IRQ, true);
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

void usb_xfer_pkt(struct usb_endpoint *ep, const uint8_t *buf, uint16_t len) {
    assert(len <= 64);

    // Prepare buffer control register value
    uint32_t val = len | USB_BUF_CTRL_AVAIL;

    if (ep_is_tx(ep)) {
        // Need to copy the data from the user buffer to the usb memory
        memcpy((void *)ep->data_buffer, (void *)buf, len);
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

    // val |= USB_BUF_CTRL_LAST;

    *ep->buffer_control = val & ~USB_BUF_CTRL_AVAIL;

    busy_wait_at_least_cycles(12);

    *ep->buffer_control = val;
}

/**
 * @param ep
 * @param buf
 * @param len
 */
void usb_xfer(struct usb_endpoint *ep, const uint8_t *buf, const uint16_t len) {
    if (len == 0) {
        usb_xfer_pkt(ep, buf, len);
    }

    uint16_t remaining_len = len;
    size_t ep_buf_size = ep->descriptor->wMaxPacketSize;

    while (remaining_len > 0) {
        size_t pkt_size = MIN(remaining_len, ep_buf_size);

        usb_xfer_pkt(ep, buf, pkt_size);

        while (*ep->buffer_control & USB_BUF_CTRL_FULL) {
            tight_loop_contents();
        }

        buf += pkt_size;
        remaining_len -= pkt_size;
    }
}

void usb_control_xfer(usb_device_t *const device, uint8_t *buf, uint16_t len) {
    usb_xfer(&device->ep0_out, buf, len);
}

void usb_xfer_ep0_in(usb_device_t *const device, const uint8_t *buf, const uint16_t len) {
    usb_xfer(&device->ep0_in, buf, len);
}

/**
 * @brief Handle a BUS RESET from the host by setting the device address back to 0.
 */
void usb_bus_reset(usb_device_t *const device) {
    // Set address back to 0
    device->addr = 0;
    device->should_set_addr = false;
    usb_hw->dev_addr_ctrl = 0;
    device->configured = false;
}

/**
 * @brief Sends a zero length status packet back to the host.
 */
void usb_acknowledge_out_request(void) {
    usb_xfer(&pico.ep0_in, NULL, 0);
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

    // Reset PID to 1 for EP0 IN
    pico.ep0_in.next_pid = 1u;

    switch (pkt->bmRequestType) {
        case USB_DIR_OUT: {
            switch (pkt->bRequest) {
                case USB_REQUEST_SET_ADDRESS:
                    usb_set_device_address(pkt);
                    break;
                case USB_REQUEST_SET_CONFIGURATION:
                    usb_set_device_configuration(pkt);
                    break;
                default:
                    usb_acknowledge_out_request();
            }

            break;
        }
        case USB_DIR_IN: {
            if (pkt->bRequest == USB_REQUEST_GET_DESCRIPTOR) {
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
                }
            }

            break;
        }
        case 0x21: {
            switch (pkt->bRequest) {
                case USB_REQUEST_SET_IDLE:
                    usb_acknowledge_out_request();
                    break;
                case USB_REQUEST_SET_REPORT:
                    struct usb_endpoint *ep = &(pico.hid_interfaces[0].endpoint);
                    set_report_callback(ep->data_buffer, pkt->wLength);
                    usb_acknowledge_out_request();
                    break;
            }

            break;
        }
        case EP_IN_HID: {
            if (pkt->bRequest == USB_REQUEST_GET_DESCRIPTOR) {
                uint16_t descriptor_type = pkt->wValue >> 8;

                if (descriptor_type == USB_DESCRIPTOR_TYPE_REPORT) {
                    usb_handle_hid_report(pkt->bRequest, pkt->wIndex, pkt->wLength);
                }
            }

            break;
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

    struct usb_endpoint *endpoints[4] = {
        &pico.ep0_in,
        &pico.ep0_out,
        &pico.hid_interfaces[0].endpoint,
        &pico.hid_interfaces[1].endpoint,
    };

    for (uint i = 0; i < 3; i++) {
        struct usb_endpoint *ep = endpoints[i];
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

void usb_suspend_callback(void) {

}

void usb_resume_callback(void) {

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
        usb_handle_setup_packet();
        usb_hw_clear->sie_status = USB_SIE_STATUS_SETUP_REC_BITS;
    }

    // Buffer status, one or more buffers have completed
    if (status & USB_INTS_BUFF_STATUS_BITS) {
        usb_handle_buff_status();
        handled |= USB_INTS_BUFF_STATUS_BITS;
    }

    // Bus is reset
    if (status & USB_INTS_BUS_RESET_BITS) {
        handled |= USB_INTS_BUS_RESET_BITS;
        usb_bus_reset(&pico);
        usb_hw_clear->sie_status = USB_SIE_STATUS_BUS_RESET_BITS;
    }

    // Device is suspended
    if (status & USB_INTS_DEV_SUSPEND_BITS) {
        handled |= USB_INTS_DEV_SUSPEND_BITS;

        if (pico.configured && !pico.suspended) {
            pico.suspended = true;
            usb_suspend_callback();
        }

        usb_hw_clear->sie_status = USB_SIE_STATUS_SUSPENDED_BITS;
    }

    // Device is resumed
    if (status & USB_INTS_DEV_RESUME_FROM_HOST_BITS) {
        handled |= USB_INTS_DEV_RESUME_FROM_HOST_BITS;

        if (pico.configured && pico.suspended) {
            pico.suspended = false;
            usb_resume_callback();
        }

        usb_hw_clear->sie_status = USB_SIE_STATUS_RESUME_BITS;
    }

    if (status ^ handled) {
        panic("Unhandled IRQ 0x%x\n", (uint) (status ^ handled));
    }
}

void set_report_callback(volatile uint8_t *buf, uint16_t len) {

}

/**
 * @brief send device descriptor to host
 * @param wLength
 */
void usb_handle_device_descriptor(const uint16_t wLength) {
    // Always respond with pid 1
    pico.ep0_in.next_pid = 1;
    usb_xfer_ep0_in(&pico, (uint8_t*)pico.device_descriptor, pico.device_descriptor->bLength);
}

uint hid_interface_cpy(uint8_t *buffer, const struct usb_hid_interface *const hid_interface) {
    memcpy((void *)buffer, hid_interface->interface_descriptor, hid_interface->interface_descriptor->bLength);
    uint offset = hid_interface->interface_descriptor->bLength;

    memcpy((void *)buffer + offset, hid_interface->hid_descriptor, hid_interface->hid_descriptor->bLength);
    offset += hid_interface->hid_descriptor->bLength;

    memcpy((void *)buffer + offset, hid_interface->endpoint.descriptor, hid_interface->endpoint.descriptor->bLength);
    offset += hid_interface->endpoint.descriptor->bLength;

    return offset;
}

void usb_handle_config_descriptor(const uint16_t wLength) {
    uint8_t buffer[128];
    uint8_t *buffer_ptr = buffer;

    memcpy((void *)buffer_ptr, pico.configuration_descriptor, pico.configuration_descriptor->bLength);
    buffer_ptr += pico.configuration_descriptor->bLength;

    if (wLength >= pico.configuration_descriptor->wTotalLength) {
        // keyboard
        buffer_ptr += hid_interface_cpy(buffer_ptr, &pico.hid_interfaces[0]);
        // mouse
        // buffer_ptr += hid_interface_cpy(buffer_ptr, &pico.hid_interfaces[1]);
    }

    usb_xfer_ep0_in(&pico, buffer, buffer_ptr - buffer);
}

void utf8_to_utf16(const char *utf8, const uint utf8_len, uint16_t *utf16_buffer) {
    for (uint32_t i = 0; i < utf8_len; i++) utf16_buffer[i] = utf8[i];
}

/**
 * @param descriptorIndex
 * @param languageId
 */
void usb_handle_string_descriptor(const uint8_t descriptorIndex, const uint16_t languageId) {
    if (descriptorIndex == 0) {
        usb_xfer_ep0_in(&pico, (uint8_t *)pico.language_descriptor, 4);
    }
    else {
        char const **string = (descriptorIndex == 1) ? &pico.vendor : &pico.product;
        const uint string_len = strlen(*string);
        const uint8_t bLength = 2 + string_len * 2;

        uint16_t string_descriptor_buffer[40];
        string_descriptor_buffer[0] = USB_DESCRIPTOR_TYPE_STRING << 8 | bLength;
        utf8_to_utf16(*string, string_len, &string_descriptor_buffer[1]);

        usb_xfer_ep0_in(&pico, (uint8_t *)string_descriptor_buffer, bLength);
    }
}

/**
 * @param bDescriptorIndex - descriptor id for this interface
 * @param InterfaceNumber - interface id
 * @param wDescriptorLength - request length
 */
void usb_handle_hid_report(const uint16_t bDescriptorIndex, const uint16_t wInterfaceNumber, const uint16_t wDescriptorLength) {
    switch (wInterfaceNumber) {
        case 0:
            usb_xfer_ep0_in(&pico, (uint8_t *)keyboard_report_descriptor, sizeof(keyboard_report_descriptor));
            break;
        case 1:
            usb_xfer_ep0_in(&pico, (uint8_t *)mouse_report_descriptor, sizeof(mouse_report_descriptor));
            break;
    }
}

void ep0_out_handler(uint8_t *buf, uint16_t len) {

}

/**
 * @brief EP0 in transfer complete. Either finish the SET_ADDRESS process, or receive a zero
 * length status packet from the host.
 * @param buf the data that was sent
 * @param len the length that was sent
 */
void ep0_in_handler(uint8_t *buf, uint16_t len) {
    if (pico.should_set_addr) {
        // Set actual device address in hardware
        usb_hw->dev_addr_ctrl = pico.addr;
        pico.should_set_addr = false;
    } else {
        // Receive a zero length status packet from the host on EP0 OUT
        usb_xfer(&pico.ep0_out, NULL, 0);
    }
}

void ep1_in_handler(uint8_t *buf, uint16_t len) {

}

void ep2_in_handler(uint8_t *buf, uint16_t len) {

}
