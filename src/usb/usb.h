/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef USB_H
#define USB_H

#include "usb_common.h"
#include "usb_hid.h"

#include <string.h>

// Function prototypes for our device specific endpoint handlers defined
// later on
void ep0_in_handler(uint8_t *buf, uint16_t len);
void ep0_out_handler(uint8_t *buf, uint16_t len);
void ep1_out_handler(uint8_t *buf, uint16_t len);
void ep2_in_handler(uint8_t *buf, uint16_t len);

void ep1_in_hid_handler(uint8_t *buf, uint16_t len);

bool is_configured(void);

struct usb_endpoint_configuration *usb_get_endpoint_configuration(uint8_t addr);

static inline uint32_t usb_buffer_offset(volatile uint8_t *buf);

void usb_setup_endpoint(const struct usb_endpoint_configuration *ep);

void usb_setup_endpoints(void);

void usb_device_init(void);

static inline bool ep_is_tx(struct usb_endpoint_configuration *ep);

void usb_start_transfer(struct usb_endpoint_configuration *ep, uint8_t *buf, uint16_t len);

void usb_start_great_transfer(struct usb_endpoint_configuration *ep, uint8_t *buf, uint16_t len);

void usb_send_hid_keyboard_report(struct usb_hid_keyboard_report *keyboard_report);

void usb_send_hid_consumer_control_report(struct usb_hid_consumer_control_report *consumer_control_report);

void usb_handle_device_descriptor(volatile struct usb_setup_packet *pkt);

void usb_handle_report_descriptor(volatile struct usb_setup_packet *pkt);

void usb_handle_config_descriptor(volatile struct usb_setup_packet *pkt);

void usb_bus_reset(void);

void usb_handle_string_descriptor(volatile struct usb_setup_packet *pkt);

void usb_acknowledge_out_request(void);

void usb_set_device_address(volatile struct usb_setup_packet *pkt);

void usb_set_device_configuration(volatile struct usb_setup_packet *pkt);

void usb_handle_setup_packet(void);

static void usb_handle_ep_buff_done(struct usb_endpoint_configuration *ep);

static void usb_handle_buff_done(uint ep_num, bool in);

static void usb_handle_buff_status(void);

typedef void (*usb_ep_handler)(uint8_t *buf, uint16_t len);

// out: from the host to the device
// in: from the device to the host
#define EP0_IN_ADDR  (USB_DIR_IN  | 0) // 0x80
#define EP0_OUT_ADDR (USB_DIR_OUT | 0) // 0x00
#define EP1_IN_ADDR  (USB_DIR_IN  | 1) // 0x81

#define EP0_BUF_SIZE 64
#define EP1_BUF_SIZE 64

// endpoints descriptors ******************************************
static const struct usb_endpoint_descriptor ep0_out = {
        .bLength          = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = EP0_OUT_ADDR, // EP number 0, OUT from host (rx to device)
        .bmAttributes     = USB_TRANSFER_TYPE_CONTROL,
        .wMaxPacketSize   = EP0_BUF_SIZE,
        .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep0_in = {
        .bLength          = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = EP0_IN_ADDR, // EP number 0, OUT from host (rx to device)
        .bmAttributes     = USB_TRANSFER_TYPE_CONTROL,
        .wMaxPacketSize   = EP0_BUF_SIZE,
        .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep1_in_hid = {
        .bLength          = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = EP1_IN_ADDR,
        .bmAttributes     = USB_TRANSFER_TYPE_INTERRUPT,
        .wMaxPacketSize   = EP1_BUF_SIZE,
        .bInterval        = 10 //10 ms, was 5ms
};
// ****************************************************************

// device descriptor ******************************************
static const struct usb_device_descriptor device_descriptor = {
        .bLength            = sizeof(struct usb_device_descriptor),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_DEVICE,
        .bcdUSB             = 0x0110, // USB 1.1 device
        .bDeviceClass       = 0,      // Specified in interface descriptor
        .bDeviceSubClass    = 0,      // No subclass
        .bDeviceProtocol    = 0,      // No protocol
        .bMaxPacketSize0    = EP0_BUF_SIZE, // Max packet size for ep0
        .idVendor           = 0x2137, // Your vendor id
        .idProduct          = 0xB426, // Your product ID
        .bcdDevice          = 0x0100,      // No device revision number
        .iManufacturer      = 1,      // Manufacturer string index
        .iProduct           = 2,      // Product string index
        .iSerialNumber      = 0,        // No serial number
        .bNumConfigurations = 1    // One configuration
};
// ***********************************************************


// hid interface descriptor *********************************************
static const struct usb_interface_descriptor hid_interface_descriptor = {
        .bLength            = sizeof(struct usb_interface_descriptor),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
        .bInterfaceNumber   = 0,
        .bAlternateSetting  = 0,
        .bNumEndpoints      = 0x01,    // Interface has 1 endpoints
        .bInterfaceClass    = INTERFACE_CLASS_TYPE_HID, // Vendor specific endpoint
        .bInterfaceSubClass = 0x01, // boot interface
        .bInterfaceProtocol = 0x01, // keyboard
        .iInterface         = 0
};
// **********************************************************************

// string descriptors ******************************************
#define NUM_STRING_DESCRIPTORS 3
#define VENDOR "Seegson Corporation"
#define PRODUCT "KB-426"
#define STRING_DESCRIPTOR_SIZE(bString) (0x02 + (strlen(bString) * 2))

static const struct usb_string_descriptor lang_descriptor = {
    .bLength         = 0x04,
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .wLANGID         = 0x0409
};

static const struct usb_string_descriptor vendor_descriptor = {
    .bLength         = STRING_DESCRIPTOR_SIZE(VENDOR),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .bString         = VENDOR
};

static const struct usb_string_descriptor product_descriptor = {
    .bLength         = STRING_DESCRIPTOR_SIZE(PRODUCT),
    .bDescriptorType = USB_DESCRIPTOR_TYPE_STRING,
    .bString         = PRODUCT
};
// *************************************************************


/*static const report_desc[] = {
        USAGE_PAGE, 0x01, // usage page: generi desktop control
        USAGE, 0x06, // usage: keyboard/keypad

        COLLECTION, 0x01, //start collection
                USAGE_PAGE, 0x07, // keyboard/keypad

                USAGE_MININUM, KC_CTRL_LEFT,
                USAGE_MAXIMUM, KC_GUI_RIGHT,
                LOGICAL_MINIMUM, 0x00, // unpressed key
                LOGICAL_MAXIMUM, 0x01, // pressed key

                REPORT_SIZE, 0x01, // a modifer is 1 bit
                REPORT_COUNT, 0x08, // height modifiers, therefore 1 byte
                INPUT, 0x02, // one byte i guess

                REPORT_SIZE, 0x08,
                REPORT_COUNT, 0x01,
                INPUT, 0x03,

                
};*/

#define USB_HID_REPORT_DESCRIPTOR_LENGTH 65//+ 26
static const uint8_t hid_report_descriptor[USB_HID_REPORT_DESCRIPTOR_LENGTH] = {
          // usage keyboard
        0x05, 0x01,
        0x09, 0x06,
                0xa1, 0x01,
                0x85, 0x01, // report i
                
                0x05, 0x07, // usage page keyboard/keypad
                0x19, 0xe0, // usage minimum
                0x29, 0xe7, // usage maximum
                0x15, 0x00, // logical minimum
                0x25, 0x01, // logical maximum
                0x75, 0x01, //* report size 1
                0x95, 0x08, // Report Count 8
                0x81, 0x02, // Input
                0x95, 0x01, // Report Count 1
                0x75, 0x08, // Report Size 8
                0x81, 0x01, // Input
                0x95, 0x05, // Report Count 5
                0x75, 0x01, // Report Size 1
               
                0x05, 0x08,
                0x19, 0x01,
                0x29, 0x05,
                0x91, 0x02,
                0x95, 0x01,
                0x75, 0x03, //report size
                0x91, 0x01, 
                0x95, 0x06,
                0x75, 0x08,
                0x15, 0x00,
                0x25, 0x65,

                0x05, 0x07,
                0x19, 0x00,
                0x29, 0xff,
                0x81, 0x00,

        0xc0,

        // usage consumer control
        /*0x05, 0x0c,
        0x09, 0x01,
                0xa1, 0x01,
                0x85, 0x02,       // report id
                0x15, 0x00,       // logical minimum
                0x26, 0xff, 0x03, // logical maximum
                0x19, 0x00,       // usage minimum
                0x2a, 0xff, 0x03, // usage maximum 
                0x95, 0x01,       // report count
                0x75, 0x10,       // report size
                0x81, 0x00,
        0xc0*/
};

// hid descriptor *******************************************
static const struct usb_hid_descriptor hid_descriptor = {
        .bLength         = sizeof(struct usb_hid_descriptor),
        .bDescriptorType = USB_HID_DESCRIPTOR_TYPE_HID,
        .bcdHID          = 0x0111, //usb hid version
        .bCountryCode    = USB_HID_LOCAL_FRENCH,
        .bNumDescriptors = 0x01,
        .bReportType     = USB_HID_DESCRIPTOR_TYPE_REPORT,
        .wReportLength   = sizeof(hid_report_descriptor) 
};
// **********************************************************

// configuration descriptor *********************************************
static const struct usb_configuration_descriptor config_descriptor = {
        .bLength             = sizeof(struct usb_configuration_descriptor),
        .bDescriptorType     = USB_DESCRIPTOR_TYPE_CONFIG,
        .wTotalLength        = (sizeof(struct usb_configuration_descriptor) +
                                sizeof(struct usb_interface_descriptor) +
                                sizeof(struct usb_hid_descriptor) +
                                sizeof(struct usb_endpoint_descriptor)),
        .bNumInterfaces      = 1,
        .bConfigurationValue = 1, // Configuration 1
        .iConfiguration      = 0,      // No string
        .bmAttributes        = 0xa0, // self powered, remote wakeup
        .bMaxPower           = 0x32         // 100ma
};
// **********************************************************************

// Struct in which we keep the endpoint configuration
struct usb_endpoint_configuration {
    const struct usb_endpoint_descriptor *descriptor;
    usb_ep_handler handler;

    // Pointers to endpoint + buffer control registers
    // in the USB controller DPSRAM
    volatile uint32_t *endpoint_control;
    volatile uint32_t *buffer_control;
    volatile uint8_t *data_buffer;

    // Toggle after each packet (unless replying to a SETUP)
    uint8_t next_pid;
};

// Struct in which we keep the device configuration
struct usb_device_configuration {
    const struct usb_device_descriptor *device_descriptor;
    const struct usb_interface_descriptor *interface_descriptor;
    const struct usb_configuration_descriptor *config_descriptor;
    const struct usb_hid_descriptor *hid_descriptor;
    const uint8_t *hid_report_descriptor;
    //const unsigned char *lang_descriptor;
    //const unsigned char **descriptor_strings;
    const struct usb_string_descriptor *string_descriptors[NUM_STRING_DESCRIPTORS];
    // USB num endpoints is 16
    struct usb_endpoint_configuration endpoints[USB_NUM_ENDPOINTS];
};

#endif  