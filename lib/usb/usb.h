#ifndef USB_H
#define USB_H

#include "usb_common.h"
#include "usb_descriptors.h"
#include "usb_hid.h"

// usb config
#include "usb_config.h"

#include <string.h>

void set_report_callback(volatile uint8_t *buf, uint16_t len) __attribute__((weak));

bool is_configured(void);

void usb_handle_suspend(void);

void usb_handle_resume(void);

// struct usb_endpoint *usb_get_endpoint_configuration(uint8_t addr);

static inline uint32_t usb_buffer_offset(volatile uint8_t *buf);

void usb_setup_endpoint(const struct usb_endpoint *ep);

void usb_setup_endpoints(void);

void usb_device_init(void);

static inline bool ep_is_tx(struct usb_endpoint *ep);

void usb_xfer_pkt(struct usb_endpoint *ep, uint8_t *buf, uint16_t len);

void usb_xfer(struct usb_endpoint *ep, uint8_t *buf, uint16_t len);

void usb_control_xfer(uint8_t *buf, uint16_t len);

void usb_xfer_ep0_in(uint8_t *buf, uint16_t len);

void usb_bus_reset(void);

void usb_acknowledge_out_request(void);

void usb_set_device_address(volatile struct usb_setup_packet *pkt);

void usb_set_device_configuration(volatile struct usb_setup_packet *pkt);

void usb_handle_setup_packet(void);

static void usb_handle_ep_buff_done(struct usb_endpoint *ep);

static void usb_handle_buff_done(uint ep_num, bool in);

static void usb_handle_buff_status(void);

#endif
