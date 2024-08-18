#ifndef USB_CONFIG_H
#define USB_CONFIG_H

#include "usb.h"
#include "usb_descriptors.h"

struct usb_device {
    uint8_t addr;

    bool should_set_addr;
    volatile bool configured;
    volatile bool suspended;
    const struct usb_device_descriptor *device_descriptor;
    const struct usb_configuration_descriptor *configuration_descriptor;
    const struct usb_interface_descriptor *interface_descriptor;
    const struct usb_hid_descriptor *hid_descriptor;
    struct usb_endpoint endpoints[3];

    const struct usb_string_language_descriptor *language_descriptor;
    const char *vendor;
    const char *product;

    const uint8_t *report_descriptor;
    const size_t report_descriptor_len;
};

void utf8_to_utf16(const char *utf8, const size_t utf8_len, uint16_t *utf16_buffer);

void usb_handle_device_descriptor(const uint16_t wLength);

void usb_handle_config_descriptor(const uint16_t wLength);

void usb_handle_string_descriptor(const uint8_t descriptorIndex, const uint16_t languageId);

void usb_handle_report_descriptor(const uint16_t wLength);

void ep0_in_handler(uint8_t *buf, uint16_t len);

void ep0_out_handler(uint8_t *buf, uint16_t len);

void ep1_in_handler(uint8_t *buf, uint16_t len);

#endif
