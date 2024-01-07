#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

#include <stdint.h>
#include "pico/types.h"
#include "hardware/structs/usb.h"

//-------------------------
// DEVICE DESCRIPTOR
//-------------------------
struct device_descriptor{
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
    uint8_t	iSerialNumber;
    uint8_t bNumConfigurations;
};


//-------------------------
// CONFIGURATION DESCRIPTOR
//-------------------------
struct configuration_descriptor{
	uint8_t bLength; 	
 	uint8_t bDescriptorType;  
 	uint16_t wTotalLength;  	
 	uint8_t bNumInterfaces;  	
 	uint8_t bConfigurationValue;  	
 	uint8_t iConfiguration;  	
 	uint8_t bmAttributes;  
 	uint8_t bMaxPower;  
};


//-------------------------
// INTERFACE DESCRIPTOR
//-------------------------
struct interface_descriptor{
    uint8_t bLength; // descriptor size
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;
    uint8_t bNumEndpoints;
    uint8_t bInterfaceClass;
    uint8_t bInterfaceSubClass;
    uint8_t bInterfaceProtocol;
    uint8_t iInterface;
};


//-------------------------
// ENDPOINT DESCRIPTOR
//-------------------------
struct endpoint_descriptor{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bEndpointAddress;
    uint8_t bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t bInterval;
};



//-------------------------
// STRING DESCRIPTOR
//-------------------------
struct string_descriptor{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t bString[17];
};


//-------------------------
// HID DESCRIPTOR
//-------------------------
struct hid_device_descriptor{
    uint8_t bLength;      //9
    uint8_t bDescriptorType;     //33
    uint16_t bcdHID;    //1.11
    uint8_t bCountryCode;      //0 Not supported // i think it should be 0x08 (for France)
    uint8_t bNumDescriptors;      //1
    uint8_t bDescriptorType2;     //34 Report
    uint16_t wDescriptorLength;    // 239
};


/*#ifdef __GNUC__
//-------------------------
// HID INTERFACE
//-------------------------
struct hid_interface{
    uint8_t interfaceNumber;
    uint8_t endpointIn;
    uint8_t endpointOut;
    uint8_t interfaceProtocol;
    uint8_t protocolMode;
    uint8_t idleRate;
    uint16_t reportDescriptorLength;

    __attribute__((aligned(4))) uint8_t endpointInBuffer[64];
    __attribute__((aligned(4))) uint8_t endpointOutBuffer[64];

    struct hid_device_descriptor hid_device_desc;
};
#endif*/

#endif