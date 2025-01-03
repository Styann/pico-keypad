#ifndef USB_H
#define USB_H

#include <string.h>

#include "usb_common.h"
#include "usb_descriptors.h"
#include "usb_hid.h"
#include "usb_config.h"

struct usb_hid_interface {
    const struct usb_interface_descriptor *interface_descriptor;
    const struct usb_hid_descriptor *hid_descriptor;
    struct usb_endpoint endpoint;
};

typedef struct {
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
} usb_device_t;

void set_report_callback(volatile uint8_t *buf, uint16_t len) __attribute__((weak));

void usb_suspend_callback(void) __attribute__((weak));

void usb_resume_callback(void) __attribute__((weak));

// struct usb_endpoint *usb_get_endpoint_configuration(uint8_t addr);

static inline uint32_t usb_buffer_offset(volatile uint8_t *buf);

void usb_setup_endpoint(struct usb_endpoint *ep);

void usb_setup_endpoints(usb_device_t *const device);

void usb_device_init(void);

static inline bool ep_is_tx(struct usb_endpoint *ep);

void usb_xfer_pkt(struct usb_endpoint *ep, const uint8_t *buf, uint16_t len);

void usb_xfer(struct usb_endpoint *ep, const uint8_t *buf, const uint16_t len);

void usb_control_xfer(usb_device_t *const device, uint8_t *buf, uint16_t len);

void usb_xfer_ep0_in(usb_device_t *const device, const uint8_t *buf, const uint16_t len);

void usb_bus_reset(usb_device_t *const device);

void usb_acknowledge_out_request(void);

void usb_set_device_address(volatile struct usb_setup_packet *pkt);

void usb_set_device_configuration(volatile struct usb_setup_packet *pkt);

void usb_handle_setup_packet(void);

static void usb_handle_ep_buff_done(struct usb_endpoint *ep);

static void usb_handle_buff_done(uint ep_num, bool in);

static void usb_handle_buff_status(void);

// usb config

void usb_handle_device_descriptor(const uint16_t wLength);

void utf8_to_utf16(const char *utf8, const uint utf8_len, uint16_t *utf16_buffer);

void usb_handle_config_descriptor(const uint16_t wLength);

void usb_handle_string_descriptor(const uint8_t descriptorIndex, const uint16_t languageId);

void usb_handle_hid_report(const uint16_t bDescriptorIndex, const uint16_t wInterfaceNumber, const uint16_t wDescriptorLength);

void ep0_in_handler(uint8_t *buf, uint16_t len);

void ep0_out_handler(uint8_t *buf, uint16_t len);

void ep1_in_handler(uint8_t *buf, uint16_t len);

void ep2_in_handler(uint8_t *buf, uint16_t len);

uint hid_interface_cpy(uint8_t *buffer, const struct usb_hid_interface *const hid_interface);

#endif
