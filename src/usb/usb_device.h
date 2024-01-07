#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#include "descriptor.h"

//-------------------------
// GLOBAL DEFINITION
//-------------------------
#define NUM_ENDPOINTS 1

#define RHPORT 0x00
#define RHPORT_INVALID 0xFFu

#define USB_VERSION 0x0200

#define BYTE_HIGH(byte) ((uint8_t)((byte >> 8) & 0x00ff))
#define BYTE_LOW(byte) ((uint8_t) (byte & 0x00ff))  

enum usb_transfer_type {
    USB_TRANSFER_TYPE_CONTROL = 0x00,
    USB_TRANSFER_TYPE_ISOCHRONOUS = 0x01,
    USB_TRANSFER_TYPE_BULK = 0x02,
    USB_TRANSFER_TYPE_INTERRUPT = 0x03,//this is for hid
};

enum descriptor_type {
    DEVICE_DESCRIPTOR_TYPE = 0x01,
    CONFIGURATION_DESCRIPTOR_TYPE = 0x02,
    STRING_DESCRIPTOR_TYPE = 0x03,
    INTERFACE_DESCRIPTOR_TYPE = 0x04,
    ENDPOINT_DESCRIPTOR_TYPE = 0x05,
    HID_DESCRIPTOR_TYPE = 0x21,
    REPORT_DESCRIPTOR_TYPE = 0x22
};

enum request_type {
    GET_REPORT_REQUEST_TYPE = 0x01,
    GET_IDLE_REQUEST_TYPE = 0x02,
    GET_PROTOCOL_REQUEST_TYPE = 0x03,
    SET_REPORT_REQUEST_TYPE = 0x09,
    SET_IDLE_REQUEST_TYPE = 0x0A,
    SET_PROTOCOL_REQUEST_TYPE = 0x0B
};

typedef void (*usb_endpoint_handler_t)(uint8_t *buffer, uint16_t length);
struct usb_endpoint_configuration {
    const struct endpoint_descriptor *descriptor;
    usb_endpoint_handler_t handler;
    
    volatile uint32_t *endpoint_control;
    volatile uint32_t *buffer_control;
    volatile uint32_t *data_buffer;

    uint8_t next_pid;
};

struct usb_device_configuration {
    const struct device_descriptor *device_descriptor;
    const struct interface_descriptor *interface_descriptor;
    const struct configuration_descriptor *configuration_descriptor;
    const struct string_descriptor *manufacturer_string_descriptor;
    const struct string_descriptor *product_string_descriptor;
    const struct string_descriptor *serial_number_string_descriptor;
    // USB num endpoints is 16
    struct usb_endpoint_configuration endpoints[NUM_ENDPOINTS];
};

//-------------------------
// DESCRIPTOR INIT
//-------------------------
// * don't know why but 16 bits value are inverted 0xCAFE -> 0xFECA  
struct device_descriptor pico_device_descriptor = {
    .bLength            = sizeof(struct device_descriptor), //18 bytes
    .bDescriptorType    = DEVICE_DESCRIPTOR_TYPE, //DEVICE
    .bcdUSB             = USB_VERSION, //*
    .bDeviceClass       = 0x00,
    .bDeviceSubClass    = 0x00,   // no subclass
    .bDeviceProtocol    = 0x00,   // no 
    .bMaxPacketSize0    = 0x40, //64 bytes
    .idVendor           = 0xB00B, //*
    .idProduct          = 0xF1F0, //*
    .bcdDevice          = 0x0100, //*
    .iManufacturer      = 0x01, //index manufacturer string desc 
    .iProduct           = 0x02, //index product string desc
    .iSerialNumber      = 0x03, //index serial number string desc
    .bNumConfigurations = 0x01 //one config
};

struct configuration_descriptor pico_config_descriptor = {
    .bLength             = sizeof(struct configuration_descriptor),
    .bDescriptorType     = CONFIGURATION_DESCRIPTOR_TYPE,
    .wTotalLength        = 0x0022,
    .bNumInterfaces      = 0x01,
    .bConfigurationValue = 0x01,
    .iConfiguration      = 0x00, //no string desc
    .bmAttributes        = 0xA0, //bus powered, remote-wakeup supported
    .bMaxPower           = 0x32 //100mA
};


struct interface_descriptor pico_interface_descriptor = {
    .bLength            = sizeof(struct interface_descriptor),
    .bDescriptorType    = INTERFACE_DESCRIPTOR_TYPE,
    .bInterfaceNumber   = 0x00,
    .bAlternateSetting  = 0x00,
    .bNumEndpoints      = 0x01,
    .bInterfaceClass    = 0x03, //class hid,
    .bInterfaceSubClass = 0x00, //no subclass,
    .bInterfaceProtocol = 0x00, //i think it should be 1 (stand for keyboard),
    .iInterface         = 0x00
};

struct endpoint_descriptor pico_endpoint_in_descriptor = {
    .bLength          = sizeof(struct endpoint_descriptor),
    .bDescriptorType  = ENDPOINT_DESCRIPTOR_TYPE,
    .bEndpointAddress = 0x81, //endpoint address for hid
    .bmAttributes     = USB_TRANSFER_TYPE_INTERRUPT, // xfer type interupt
    .wMaxPacketSize   = 0x0040,//64 bytes
    .bInterval        = 0x05
};

struct string_descriptor pico_manufacturer_string_descriptor = {
    .bLength         = sizeof(struct string_descriptor),
    .bDescriptorType = STRING_DESCRIPTOR_TYPE,
    .bString         = {'b','a','c','k','b','o','n','e'}
};

struct string_descriptor pico_product_string_descriptor = {
    .bLength         = sizeof(struct string_descriptor),
    .bDescriptorType = STRING_DESCRIPTOR_TYPE,
    .bString         = {'b','a','c','k','b','o','n','e',' ','k','e','y','b','o','a','r','d'}
};

struct string_descriptor pico_serial_number_string_descriptor = {
    .bLength         = sizeof(struct string_descriptor),
    .bDescriptorType = STRING_DESCRIPTOR_TYPE,
    .bString         = {'E','0','C','9','1','2','D','2','4','3','4','0'}
};

struct hid_device_descriptor pico_hid_device_descriptor = {
    .bLength           = sizeof(struct hid_device_descriptor),
    .bDescriptorType   = HID_DESCRIPTOR_TYPE,
    .bcdHID            = USB_VERSION, //U16_TO_U8S_LE
    .bCountryCode      = 0x08, // for France
    .bNumDescriptors   = 0x01,
    .bDescriptorType2  = REPORT_DESCRIPTOR_TYPE,
    .wDescriptorLength = 0x002F //size of report descriptor
};

//-------------------------
// FUNCTION
//-------------------------
void usb_device_init(void);

void usb_setup_endpoint(const struct usb_endpoint_configuration *ep);

void usb_setup_endpoints(void);


#endif