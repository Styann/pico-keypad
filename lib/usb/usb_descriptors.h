#ifndef USB_DESCRIPTORS_H
#define USB_DESCRIPTORS_H

#include <stdint.h>

// b mean byte
// w mean word
// bm mean bit-mask
// bcd mean binary-coded-decimal

enum bDescriptorTypes {
    USB_DESCRIPTOR_TYPE_DEVICE                = 0x01,
    USB_DESCRIPTOR_TYPE_CONFIG                = 0x02,
    USB_DESCRIPTOR_TYPE_STRING                = 0x03,
    USB_DESCRIPTOR_TYPE_INTERFACE             = 0x04,
    USB_DESCRIPTOR_TYPE_ENDPOINT              = 0x05,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER      = 0x06,
    USB_DESCRIPTOR_TYPE_OTHER_SPEED_CONFIG    = 0x07,
    USB_DESCRIPTOR_TYPE_INTERFACE_POWER       = 0x08,
    USB_DESCRIPTOR_TYPE_OTG                   = 0x09,
    USB_DESCRIPTOR_TYPE_DEBUG                 = 0x0A,
    USB_DESCRIPTOR_TYPE_INTERFACE_ASSOCIATION = 0x0B,
    USB_DESCRIPTOR_TYPE_HID                   = 0x21,
    USB_DESCRIPTOR_TYPE_REPORT                = 0x22,
    USB_DESCRIPTOR_TYPE_PHYSICAL              = 0x23,
    USB_DESCRIPTOR_TYPE_CS_INTERFACE          = 0x24
};

enum bmAttributes {
    USB_TRANSFER_TYPE_CONTROL     = 0x00,
    USB_TRANSFER_TYPE_ISOCHRONOUS = 0x01,
    USB_TRANSFER_TYPE_BULK        = 0x02,
    USB_TRANSFER_TYPE_INTERRUPT   = 0x03,
    USB_TRANSFER_TYPE_BITS        = 0x03
};

struct usb_device_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdUSB;
    uint8_t bDeviceClass;
    uint8_t bDeviceSubClass;
    uint8_t bDeviceProtocol;
    uint8_t bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t iManufacturer;
    uint8_t iProduct;
    uint8_t iSerialNumber;
    uint8_t bNumConfigurations;
} __packed;

struct usb_configuration_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wTotalLength;
    uint8_t bNumInterfaces;
    uint8_t bConfigurationValue;
    uint8_t iConfiguration;
    uint8_t bmAttributes;
    uint8_t bMaxPower;
} __packed;

enum bInterfaceClass {
    USB_INTERFACE_CLASS_TYPE_CDC_CONTROL  = 0x02,
    USB_INTERFACE_CLASS_TYPE_HID          = 0x03,
    USB_INTERFACE_CLASS_TYPE_CDC_DATA     = 0x0A
};

enum bInterfaceSubClass {
    USB_INTERFACE_SUBCLASS_TYPE_ABSTRACT_CONTROL_MODEL = 0x02
};

enum bInterfaceProtocol {
    USB_INTERFACE_PROTOCOL_NONE        = 0x00,
    USB_INTERFACE_PROTOCOL_KEYBOARD    = 0x01,
    USB_INTERFACE_PROTOCOL_MOUSE       = 0x02,
    USB_INTERFACE_PROTOCOL_AT_COMMANDS = 0x01
};

struct usb_interface_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
} __packed;

enum wLANGID {
    USB_LANGUAGE_ID_US              = 0x0409,
    USB_LANGUAGE_ID_FRENCH          = 0x040C,
    USB_LANGUAGE_ID_CANADIAN_FRENCH = 0x0C0C
};

struct usb_string_language_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t wLANGID;
} __packed;

struct usb_endpoint_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
} __packed;

enum bCountryCode {
    USB_COUNTRY_CODE_NOT_SUPPORTED   = 0x00,
    USB_COUNTRY_CODE_CANADIAN_FRENCH = 0x04,
    USB_COUNTRY_CODE_FRENCH          = 0x08,
    USB_COUNTRY_CODE_US              = 0x33
};

struct usb_hid_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    uint8_t bReportType;
    uint16_t wReportLength;
} __packed;

// https://usb.org/sites/default/files/hut1_4.pdf
enum usb_hid_report_descriptor_field {
    USAGE_PAGE          = 0x05,
    USAGE               = 0x09,
    COLLECTION          = 0xA1,
    END_COLLECTION      = 0xC0,
    USAGE_MINIMUM       = 0x19,
    USAGE_MAXIMUM       = 0x29,
    LOGICAL_MINIMUM     = 0x15,
    LOGICAL_MAXIMUM     = 0x25,
    PHYSICAL_MINIMUM    = 0x35,
    PHYSICAL_MAXIMUM    = 0x45,
    REPORT_SIZE         = 0x75,
    REPORT_COUNT        = 0x95,
    INPUT               = 0x81,
    OUTPUT              = 0x91,
    REPORT_ID           = 0x85,
    UNIT                = 0x65
};

enum usb_hid_report_descriptor_value {
    GENERIC_DESKTOP_PAGE   = 0x01,
    KEYBOARD_KEYPAD_PAGE   = 0x07,
    LED_PAGE               = 0x08,
    CONSUMER_PAGE          = 0x0C,
    BUTTON_PAGE            = 0x09,

    USAGE_KEYBOARD         = 0x06,
    USAGE_CONSUMER_CONTROL = 0x01,
    USAGE_JOYSTICK         = 0x04,
    USAGE_GAMEPAD          = 0x05,
    USAGE_X                = 0x30,
    USAGE_Y                = 0x31,
    USAGE_HAT_SWITCH       = 0x39,

    COLLECTION_PHYSICAL    = 0x00,
    COLLECTION_APPLICATION = 0x01,
    COLLECTION_LOGICAL     = 0x02,

    DATA_ARRAY_ABS         = 0x00,
    CONST_ARRAY_ABS        = 0x01,
    DATA_VAR_ABS           = 0x02,
    DATA_VAR_ABS_NULL      = 0x42,
    CONST_VAR_ABS          = 0x03
};

#endif
