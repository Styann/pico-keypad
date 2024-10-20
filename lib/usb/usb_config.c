#include "usb_config.h"

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
    .bNumInterfaces      = 2,
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
    .bInterfaceClass    = USB_INTERFACE_CLASS_TYPE_HID, // Vendor specific endpoint
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
    .bcdHID          = 0x0111, // usb hid version
    .bCountryCode    = USB_COUNTRY_CODE_FRENCH,
    .bNumDescriptors = 0x01,
    .bReportType     = USB_DESCRIPTOR_TYPE_REPORT,
    .wReportLength   = sizeof(keyboard_report_descriptor)
};

static const struct usb_hid_descriptor mouse_hid_descriptor = {
    .bLength         = sizeof(struct usb_hid_descriptor),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_HID,
    .bcdHID          = 0x0111, // usb hid version
    .bCountryCode    = USB_COUNTRY_CODE_FRENCH,
    .bNumDescriptors = 0x01,
    .bReportType     = USB_DESCRIPTOR_TYPE_REPORT,
    .wReportLength   = sizeof(mouse_report_descriptor)
};

struct usb_device pico = {
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
    .vendor = "Seegson Corporation",
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

/**
 * @brief send device descriptor to host
 * @param wLength
 */
void usb_handle_device_descriptor(const uint16_t wLength) {
    // Always respond with pid 1
    // struct usb_endpoint *ep = usb_get_endpoint_configuration(0x80);
    // ep->next_pid = 1;
    // usb_xfer(ep, (uint8_t *)pico.device_descriptor, pico.device_descriptor->bLength);
    usb_xfer_ep0_in((uint8_t*)pico.device_descriptor, pico.device_descriptor->bLength);
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
    uint8_t buffer[34];
    uint8_t *buffer_ptr = buffer;

    memcpy((void *)buffer_ptr, pico.configuration_descriptor, pico.configuration_descriptor->bLength);
    buffer_ptr += pico.configuration_descriptor->bLength;

    if (wLength >= pico.configuration_descriptor->wTotalLength) {
        // keyboard
        buffer_ptr += hid_interface_cpy(buffer_ptr, &pico.hid_interfaces[0]);
        // mouse
        buffer_ptr += hid_interface_cpy(buffer_ptr, &pico.hid_interfaces[1]);
    }

    usb_xfer_ep0_in(buffer, buffer_ptr - buffer);
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
        usb_xfer_ep0_in((uint8_t *)pico.language_descriptor, 4);
    }
    else {
        char const **string = (descriptorIndex == 1) ? &pico.vendor : &pico.product;
        const uint string_len = strlen(*string);
        const uint8_t bLength = 2 + string_len * 2;

        uint16_t string_descriptor_buffer[40];
        string_descriptor_buffer[0] = USB_DESCRIPTOR_TYPE_STRING << 8 | bLength;
        utf8_to_utf16(*string, string_len, &string_descriptor_buffer[1]);

        usb_xfer_ep0_in((uint8_t *)string_descriptor_buffer, bLength);
    }
}

/**
 * @param bDescriptorIndex - descriptor id for this interface
 * @param InterfaceNumber - interface id
 * @param wDescriptorLength - request length
 */
void usb_handle_hid_report(const uint16_t bDescriptorIndex, const uint16_t InterfaceNumber, const uint16_t wDescriptorLength) {
    if (InterfaceNumber == 0) {
        usb_xfer_ep0_in((uint8_t *)keyboard_report_descriptor, sizeof(keyboard_report_descriptor));
    }
    else {
        usb_xfer_ep0_in((uint8_t *)mouse_report_descriptor, sizeof(mouse_report_descriptor));
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

// #define USB_REPORT_DESCRIPTOR_LENGTH 40
// const uint8_t fightstick_report_descriptor[40] = {
//     USAGE_PAGE, GENERIC_DESKTOP_PAGE,

//     USAGE, USAGE_JOYSTICK,
//     COLLECTION, COLLECTION_APPLICATION,
//             COLLECTION, COLLECTION_LOGICAL,
//                     // axis x and y, 1 byte for each
//                     REPORT_SIZE, 0x08,
//                     REPORT_COUNT, 0x02,
//                     LOGICAL_MINIMUM, 0b10000001,
//                     LOGICAL_MAXIMUM, 0b01111111,
//                     // PHYSICAL_MINIMUM, 0x00,
//                     // PHYSICAL_MAXIMUM, 0xFF,
//                     USAGE, USAGE_X,
//                     USAGE, USAGE_Y,
//                     INPUT, DATA_VAR_ABS,

//                     REPORT_SIZE, 0x01,
//                     REPORT_COUNT, 0x08,
//                     LOGICAL_MAXIMUM, 0x01,
//                     PHYSICAL_MAXIMUM, 0x01,
//                     USAGE_PAGE, BUTTON_PAGE,
//                     USAGE_MINIMUM, 0x01,
//                     USAGE_MAXIMUM, 0x08,
//                     INPUT, DATA_VAR_ABS,
//             END_COLLECTION,
//     END_COLLECTION
// };
