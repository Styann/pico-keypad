#ifndef _USB_HID_H
#define _USB_HID_H


#define USB_HID_LOCAL_FRENCH 0x08
#define USB_HID_USAGE_PAGE_KEYBOARD 0x07


#define HID_USAGE_DESKTOP_KEYBOARD 0x06
#define HID_USAGE_DESKTOP_KEYPAD 0x07

#define EP_IN_HID 0x81 // (USB_DIR_IN | 1)

#define INTERFACE_CLASS_TYPE_HID 0x03

#define USB_HID_DESCRIPTOR_TYPE_HID 0x21
#define USB_HID_DESCRIPTOR_TYPE_REPORT 0x22
#define USB_HID_DESCRIPTOR_TYPE_PHYSICAL 0x23

#define USB_INTERFACE_PROTOCOL_NONE 0x00
#define USB_INTERFACE_PROTOCOL_KEYBOARD 0x01
#define USB_INTERFACE_PROTOCOL_MOUSE 0x02

struct usb_hid_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    
    uint16_t bcdHID;
    uint8_t bCountryCode;
    uint8_t bNumDescriptors;
    
    uint8_t bReportType;
    uint16_t wReportLength;
} __packed;

#define KEYBOARD_REPORT_ID 0x01
#define CONSUMER_CONTROL_REPORT_ID 0x02

struct usb_hid_keyboard_report {
    uint8_t modifier;
    uint8_t reserved;
    uint8_t keycode[6];
} __packed;

struct usb_hid_consumer_control_report {
    uint16_t value;
} __packed;

// USB HID KEYCODE (KC)
#define KC_NONE                 0x00
#define KC_A                    0x04
#define KC_B                    0x05
#define KC_C                    0x06
#define KC_D                    0x07
#define KC_E                    0x08
#define KC_F                    0x09
#define KC_G                    0x0A
#define KC_H                    0x0B
#define KC_I                    0x0C
#define KC_J                    0x0D
#define KC_K                    0x0E
#define KC_L                    0x0F
#define KC_M                    0x10
#define KC_N                    0x11
#define KC_O                    0x12
#define KC_P                    0x13
#define KC_Q                    0x14
#define KC_R                    0x15
#define KC_S                    0x16
#define KC_T                    0x17
#define KC_U                    0x18
#define KC_V                    0x19
#define KC_W                    0x1A
#define KC_X                    0x1B
#define KC_Y                    0x1C
#define KC_Z                    0x1D
#define KC_1                    0x1E
#define KC_2                    0x1F
#define KC_3                    0x20
#define KC_4                    0x21
#define KC_5                    0x22
#define KC_6                    0x23
#define KC_7                    0x24
#define KC_8                    0x25
#define KC_9                    0x26
#define KC_0                    0x27
#define KC_ENTER                0x28
#define KC_ESCAPE               0x29
#define KC_BACKSPACE            0x2A
#define KC_TAB                  0x2B
#define KC_SPACE                0x2C
#define KC_MINUS                0x2D // -
#define KC_EQUAL                0x2E // =
#define KC_BRACKET_LEFT         0x2F // { [
#define KC_BRACKET_RIGHT        0x30 // } ]
#define KC_BACKSLASH            0x31
#define KC_EUROPE_1             0x32
#define KC_SEMICOLON            0x33 // ; :
#define KC_APOSTROPHE           0x34
#define KC_GRAVE                0x35 // ` ~
#define KC_COMMA                0x36 // , <
#define KC_PERIOD               0x37 // . >
#define KC_SLASH                0x38
#define KC_CAPS_LOCK            0x39
#define KC_F1                   0x3A
#define KC_F2                   0x3B
#define KC_F3                   0x3C
#define KC_F4                   0x3D
#define KC_F5                   0x3E
#define KC_F6                   0x3F
#define KC_F7                   0x40
#define KC_F8                   0x41
#define KC_F9                   0x42
#define KC_F10                  0x43
#define KC_F11                  0x44
#define KC_F12                  0x45
#define KC_PRINT_SCREEN         0x46
#define KC_SCROLL_LOCK          0x47
#define KC_PAUSE                0x48
#define KC_INSERT               0x49
#define KC_HOME                 0x4A
#define KC_PAGE_UP              0x4B
#define KC_DELETE               0x4C
#define KC_END                  0x4D
#define KC_PAGE_DOWN            0x4E
#define KC_ARROW_RIGHT          0x4F
#define KC_ARROW_LEFT           0x50
#define KC_ARROW_DOWN           0x51
#define KC_ARROW_UP             0x52
#define KC_NUM_LOCK             0x53
#define KC_KEYPAD_DIVIDE        0x54
#define KC_KEYPAD_MULTIPLY      0x55
#define KC_KEYPAD_SUBTRACT      0x56
#define KC_KEYPAD_ADD           0x57
#define KC_KEYPAD_ENTER         0x58
#define KC_KEYPAD_1             0x59
#define KC_KEYPAD_2             0x5A
#define KC_KEYPAD_3             0x5B
#define KC_KEYPAD_4             0x5C
#define KC_KEYPAD_5             0x5D
#define KC_KEYPAD_6             0x5E
#define KC_KEYPAD_7             0x5F
#define KC_KEYPAD_8             0x60
#define KC_KEYPAD_9             0x61
#define KC_KEYPAD_0             0x62
#define KC_KEYPAD_DECIMAL       0x63
#define KC_EUROPE_2             0x64
#define KC_APPLICATION          0x65
#define KC_POWER                0x66
#define KC_KEYPAD_EQUAL         0x67
#define KC_F13                  0x68
#define KC_F14                  0x69
#define KC_F15                  0x6A
#define KC_F16                  0x6B
#define KC_F17                  0x6C
#define KC_F18                  0x6D
#define KC_F19                  0x6E
#define KC_F20                  0x6F
#define KC_F21                  0x70
#define KC_F22                  0x71
#define KC_F23                  0x72
#define KC_F24                  0x73
#define KC_EXECUTE              0x74
#define KC_HELP                 0x75
#define KC_MENU                 0x76
#define KC_SELECT               0x77
#define KC_STOP                 0x78
#define KC_AGAIN                0x79
#define KC_UNDO                 0x7A
#define KC_CUT                  0x7B
#define KC_COPY                 0x7C
#define KC_PASTE                0x7D
#define KC_FIND                 0x7E
#define KC_MUTE                 0x7F
#define KC_VOLUME_UP            0x80
#define KC_VOLUME_DOWN          0x81
#define KC_LOCKING_CAPS_LOCK    0x82
#define KC_LOCKING_NUM_LOCK     0x83
#define KC_LOCKING_SCROLL_LOCK  0x84
#define KC_KEYPAD_COMMA         0x85
#define KC_KEYPAD_EQUAL_SIGN    0x86
#define KC_KANJI1               0x87
#define KC_KANJI2               0x88
#define KC_KANJI3               0x89
#define KC_KANJI4               0x8A
#define KC_KANJI5               0x8B
#define KC_KANJI6               0x8C
#define KC_KANJI7               0x8D
#define KC_KANJI8               0x8E
#define KC_KANJI9               0x8F
#define KC_LANG1                0x90
#define KC_LANG2                0x91
#define KC_LANG3                0x92
#define KC_LANG4                0x93
#define KC_LANG5                0x94
#define KC_LANG6                0x95
#define KC_LANG7                0x96
#define KC_LANG8                0x97
#define KC_LANG9                0x98
#define KC_ALTERNATE_ERASE      0x99
#define KC_SYSREQ_ATTENTION     0x9A
#define KC_CANCEL               0x9B
#define KC_CLEAR                0x9C
#define KC_PRIOR                0x9D
#define KC_RETURN               0x9E
#define KC_SEPARATOR            0x9F
#define KC_OUT                  0xA0
#define KC_OPER                 0xA1
#define KC_CLEAR_AGAIN          0xA2
#define KC_CRSEL_PROPS          0xA3
#define KC_EXSEL                0xA4
// RESERVED					    0xA5-DF
#define KC_CTRL_LEFT            0xE0
#define KC_SHIFT_LEFT           0xE1
#define KC_ALT_LEFT             0xE2
#define KC_GUI_LEFT             0xE3
#define KC_CTRL_RIGHT           0xE4
#define KC_SHIFT_RIGHT          0xE5
#define KC_ALT_RIGHT            0xE6
#define KC_GUI_RIGHT            0xE7

// MODIFIERS
#define KC_MOD_CTRL_LEFT   0x01 << 0 //0b00000001
#define KC_MOD_SHIFT_LEFT  0x01 << 1 //0b00000010
#define KC_MOD_ALT_LEFT    0x01 << 2 //0b00000100
#define KC_MOD_GUI_LEFT    0x01 << 3 //0b00001000
#define KC_MOD_CTRL_RIGHT  0x01 << 4 //0b00010000
#define KC_MOD_SHIFT_RIGHT 0x01 << 5 //0b00100000
#define KC_MOD_ALT_RIGHT   0x01 << 6 //0b01000000
#define KC_MOD_GUI_RIGHT   0x01 << 7 //0b10000000

// USB HID CONSUMER CONTROL
/* 
    OOC -> On/Off Control
    OSC -> One Shot Control
    LC  -> Linear Control
    RTC -> Re-Trigger Control 
*/
#define KC_MEDIA_PLAY_PAUSE       0x00dd
#define KC_MEDIA_VOLUME_INCREMENT 0x00e9
#define KC_MEDIA_VOLUME_DECREMENT 0x00ea
#define KC_MEDIA_VOLUME           0x0000 

#endif