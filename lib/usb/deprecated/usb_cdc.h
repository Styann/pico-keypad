static const struct usb_endpoint_descriptor ep2_cdc_control_in = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = 0x82,
        .bmAttributes     = USB_TRANSFER_TYPE_BULK,
        .wMaxPacketSize   = 64,
        .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep3_cdc_data_in = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = 0x83,
        .bmAttributes     = USB_TRANSFER_TYPE_BULK,
        .wMaxPacketSize   = 64,
        .bInterval        = 0
};

static const struct usb_endpoint_descriptor ep3_cdc_data_out = {
        .bLength = sizeof(struct usb_endpoint_descriptor),
        .bDescriptorType  = USB_DESCRIPTOR_TYPE_ENDPOINT,
        .bEndpointAddress = 0x02,
        .bmAttributes     = USB_TRANSFER_TYPE_BULK,
        .wMaxPacketSize   = 64,
        .bInterval        = 0
};

static const struct usb_interface_descriptor cdc_control_interface_descriptor = {
        .bLength            = sizeof(struct usb_interface_descriptor),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
        .bInterfaceNumber   = 1,
        .bAlternateSetting  = 0,
        .bNumEndpoints      = 1,
        .bInterfaceClass    = USB_INTERFACE_CLASS_TYPE_CDC_CONTROL,
        .bInterfaceSubClass = INTERFACE_SUBCLASS_TYPE_ABSTRACT_CONTROL_MODEL,
        .bInterfaceProtocol = USB_INTERFACE_PROTOCOL_AT_COMMANDS, // arduino Serial = 0
        .iInterface         = 0
};

static const struct usb_interface_descriptor cdc_data_interface_descriptor = {
        .bLength            = sizeof(struct usb_interface_descriptor),
        .bDescriptorType    = USB_DESCRIPTOR_TYPE_INTERFACE,
        .bInterfaceNumber   = 2,
        .bAlternateSetting  = 0,
        .bNumEndpoints      = 2,
        .bInterfaceClass    = USB_INTERFACE_CLASS_TYPE_CDC_DATA,
        .bInterfaceSubClass = 0,
        .bInterfaceProtocol = 0,
        .iInterface         = 0
};

static const struct usb_communication_descriptor header_func_descriptor = {
        .bLength = 5,
        .bDescriptorType = USB_DESCRIPTOR_TYPE_CS_INTERFACE,
        .bDescriptorSubType = 0,
        .wData = 0x0110
};

static const struct usb_communication_descriptor acm_func_descriptor = {
        .bLength = 4,
        .bDescriptorType = USB_DESCRIPTOR_TYPE_CS_INTERFACE,
        .bDescriptorSubType = 0x02, // abstract control management func desc
        .bData = 0x06
};

static const struct usb_communication_descriptor union_func_descriptor = {
        .bLength = 5,
        .bDescriptorType = USB_DESCRIPTOR_TYPE_CS_INTERFACE,
        .bDescriptorSubType = 0x06, // union func desc
        .wData = 0x0201
};

static const struct usb_communication_descriptor call_management_func_descriptor = {
        .bLength = 5,
        .bDescriptorType = USB_DESCRIPTOR_TYPE_CS_INTERFACE,
        .bDescriptorSubType = 0x01, // call management func desc
        .wData = 0x0101
};


struct usb_interface_association_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bFirstInterface;
    uint8_t bInterfaceCount;
    uint8_t bFunctionClass;
    uint8_t bFunctionSubClass;
    uint8_t bFunctionProtocol;
    uint8_t iFunction;
} __packed;

struct usb_communication_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bDescriptorSubType;
    union {
        uint8_t bData;
        uint16_t wData;
    };
} __packed;