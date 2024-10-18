#ifndef USB_CONFIG_H
#define USB_CONFIG_H

#include "usb.h"
#include "usb_descriptors.h"

struct usb_hid_interface {
    const struct usb_interface_descriptor *interface_descriptor;
    const struct usb_hid_descriptor *hid_descriptor;
    struct usb_endpoint endpoint;
};

struct usb_device {
    uint8_t addr;

    bool should_set_addr;
    volatile bool configured;
    volatile bool suspended;
    const struct usb_device_descriptor *device_descriptor;
    const struct usb_configuration_descriptor *configuration_descriptor;

    // const struct usb_interface_descriptor *interface_descriptor;
    // const struct usb_hid_descriptor *hid_descriptor;

    struct usb_hid_interface hid_interfaces[2];

    // struct usb_endpoint endpoints[3];

    struct usb_endpoint ep0_in;
    struct usb_endpoint ep0_out;

    const struct usb_string_language_descriptor *language_descriptor;
    const char *vendor;
    const char *product;
};

void utf8_to_utf16(const char *utf8, const uint utf8_len, uint16_t *utf16_buffer);

void usb_handle_device_descriptor(const uint16_t wLength);

void usb_handle_config_descriptor(const uint16_t wLength);

void usb_handle_string_descriptor(const uint8_t descriptorIndex, const uint16_t languageId);

void usb_handle_hid_report(const uint16_t bDescriptorIndex, const uint16_t InterfaceNumber, const uint16_t wDescriptorLength);

void ep0_in_handler(uint8_t *buf, uint16_t len);

void ep0_out_handler(uint8_t *buf, uint16_t len);

void ep1_in_handler(uint8_t *buf, uint16_t len);

void ep2_in_handler(uint8_t *buf, uint16_t len);

uint hid_interface_cpy(uint8_t *buffer, const struct usb_hid_interface *const hid_interface);

#endif
