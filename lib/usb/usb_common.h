#ifndef USB_COMMON_H
#define USB_COMMON_H

#include "pico/types.h"
#include "hardware/structs/usb.h"

#define USB_DIR_OUT 0x00u
#define USB_DIR_IN 0x80u

#define EP0_OUT_ADDR 0x00u
#define EP0_IN_ADDR 0x80u

// bmRequestType bit definitions
#define USB_REQ_TYPE_STANDARD 0x00u
#define USB_REQ_TYPE_TYPE_MASK 0x60u
#define USB_REQ_TYPE_TYPE_CLASS 0x20u
#define USB_REQ_TYPE_TYPE_VENDOR 0x40u
#define USB_REQ_TYPE_RECIPIENT_MASK 0x1fu
#define USB_REQ_TYPE_RECIPIENT_DEVICE 0x00u
#define USB_REQ_TYPE_RECIPIENT_INTERFACE 0x01u
#define USB_REQ_TYPE_RECIPIENT_ENDPOINT 0x02u

// bRequest
#define USB_REQUEST_GET_STATUS        0x00
#define USB_REQUEST_CLEAR_FEATURE     0x01
#define USB_REQUEST_SET_FEATURE       0x03
#define USB_REQUEST_SET_ADDRESS       0x05
#define USB_REQUEST_GET_DESCRIPTOR    0x06
#define USB_REQUEST_SET_DESCRIPTOR    0x07
#define USB_REQUEST_GET_CONFIGURATION 0x08
#define USB_REQUEST_SET_CONFIGURATION 0x09
#define USB_REQUEST_GET_INTERFACE     0x0a
#define USB_REQUEST_SET_INTERFACE     0x0b
#define USB_REQUEST_SYNC_FRAME        0x0c
// hid
#define USB_REQUEST_GET_IDLE          0x02
#define USB_REQUEST_SET_IDLE          0x0a
#define USB_REQUEST_SET_REPORT        0x09

struct usb_setup_packet {
    uint8_t bmRequestType;
    uint8_t bRequest;
    uint16_t wValue;
    uint16_t wIndex;
    uint16_t wLength;
} __packed;

// Struct in which we keep the endpoint configuration
struct usb_endpoint {
        const struct usb_endpoint_descriptor *descriptor;
        void (*handler)(uint8_t *buf, uint16_t len);

        // Pointers to endpoint + buffer control registers
        // in the USB controller DPSRAM
        volatile uint32_t *endpoint_control;
        volatile uint32_t *buffer_control;
        volatile uint8_t *data_buffer;

        // Toggle after each packet (unless replying to a SETUP)
        uint8_t next_pid;
};

#endif
