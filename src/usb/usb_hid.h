#ifndef USB_HID_H
#define USB_HID_H

// usb hid descriptors types *****************
#define USB_HID_DESCRIPTOR_TYPE_HID       0x21
#define USB_HID_DESCRIPTOR_TYPE_REPORT    0x22
#define USB_HID_DESCRIPTOR_TYPE_PHYSICAL  0x23
// *******************************************

// usb hid requests types ****************
#define USB_HID_REQUEST_GET_IDLE      0x02
#define USB_HID_REQUEST_SET_IDLE      0x0a
#define USB_HID_REQUEST_SET_REPORT    0x09
// ***************************************

// usb report descriptor field ****
#define USAGE_PAGE          0x05
#define USAGE               0x09
#define COLLECTION          0xA1
#define END_COLLECTION      0xC0
#define USAGE_MINIMUM       0x19
#define USAGE_MAXIMUM       0x29
#define LOGICAL_MINIMUM     0x15
#define LOGICAL_MAXIMUM     0x25
#define PHYSICAL_MINIMUM    0x35
#define PHYSICAL_MAXIMUM    0x45
#define REPORT_SIZE         0x75
#define REPORT_COUNT        0x95
#define INPUT               0x81
#define OUTPUT              0x91
#define REPORT_ID           0x85
#define UNIT                0x65

//usb report descriptor value
#define GENERIC_DESKTOP_PAGE   0x01
#define KEYBOARD_KEYPAD_PAGE   0x07
#define LED_PAGE               0x08
#define CONSUMER_PAGE          0x0C
#define BUTTON_PAGE            0x09

#define USAGE_KEYBOARD         0x06
#define USAGE_CONSUMER_CONTROL 0x01
#define USAGE_JOYSTICK         0x04
#define USAGE_GAMEPAD          0x05
#define USAGE_X                0x30
#define USAGE_Y                0x31
#define USAGE_HAT_SWITCH       0x39

#define COLLECTION_PHYSICAL    0x00
#define COLLECTION_APPLICATION 0x01
#define COLLECTION_LOGICAL     0x02

#define DATA_ARRAY_ABS         0x00
#define CONST_ARRAY_ABS        0x01
#define DATA_VAR_ABS           0x02
#define DATA_VAR_ABS_NULL      0x42
#define CONST_VAR_ABS          0x03

// engine rotation, angular position
#define UNIT_ENG_ROT_ANG_POS   0x14
// ********************************

#define USB_HID_LOCAL_FRENCH 0x08
#define USB_HID_USAGE_PAGE_KEYBOARD 0x07


#define HID_USAGE_DESKTOP_KEYBOARD 0x06
#define HID_USAGE_DESKTOP_KEYPAD 0x07

#define EP_IN_HID 0x81 // (USB_DIR_IN | 1)

#define INTERFACE_CLASS_TYPE_HID 0x03

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

struct usb_gamepad_report {
    // uint8_t id;
    uint8_t x;
    uint8_t y;
    uint8_t buttons;
} __packed;

struct usb_keyboard_report {
    uint8_t id;
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keycodes[6];
} __packed;

struct usb_consumer_control_report {
    uint8_t id;
    uint16_t consumer_control;
} __packed;

// USB HID KEYCODE (KC)
#define KC_NONE                       0x00
#define KC_ERROR_ROLL_OVER            0x01
#define KC_POSTFAIL                   0x02
#define KC_ERROR_UNDEFINED            0x03  
#define KC_A                          0x04
#define KC_B                          0x05
#define KC_C                          0x06
#define KC_D                          0x07
#define KC_E                          0x08
#define KC_F                          0x09
#define KC_G                          0x0A
#define KC_H                          0x0B
#define KC_I                          0x0C
#define KC_J                          0x0D
#define KC_K                          0x0E
#define KC_L                          0x0F
#define KC_M                          0x10
#define KC_N                          0x11
#define KC_O                          0x12
#define KC_P                          0x13
#define KC_Q                          0x14
#define KC_R                          0x15
#define KC_S                          0x16
#define KC_T                          0x17
#define KC_U                          0x18
#define KC_V                          0x19
#define KC_W                          0x1A
#define KC_X                          0x1B
#define KC_Y                          0x1C
#define KC_Z                          0x1D
#define KC_1                          0x1E
#define KC_2                          0x1F
#define KC_3                          0x20
#define KC_4                          0x21
#define KC_5                          0x22
#define KC_6                          0x23
#define KC_7                          0x24
#define KC_8                          0x25
#define KC_9                          0x26
#define KC_0                          0x27
#define KC_ENTER                      0x28
#define KC_ESCAPE                     0x29
#define KC_BACKSPACE                  0x2A
#define KC_TAB                        0x2B
#define KC_SPACE                      0x2C
#define KC_MINUS                      0x2D // -
#define KC_EQUAL                      0x2E // =
#define KC_SQUARE_BRACKET_LEFT        0x2F // { [
#define KC_SQUARE_BRACKET_RIGHT       0x30 // } ]
#define KC_BACKSLASH                  0x31
#define KC_EUROPE_1                   0x32
#define KC_SEMICOLON                  0x33 // ; :
#define KC_APOSTROPHE                 0x34
#define KC_GRAVE                      0x35 // ` ~
#define KC_COMMA                      0x36 // , <
#define KC_PERIOD                     0x37 // . >
#define KC_SLASH                      0x38
#define KC_CAPS_LOCK                  0x39
#define KC_F1                         0x3A
#define KC_F2                         0x3B
#define KC_F3                         0x3C
#define KC_F4                         0x3D
#define KC_F5                         0x3E
#define KC_F6                         0x3F
#define KC_F7                         0x40
#define KC_F8                         0x41
#define KC_F9                         0x42
#define KC_F10                        0x43
#define KC_F11                        0x44
#define KC_F12                        0x45
#define KC_PRINT_SCREEN               0x46
#define KC_SCROLL_LOCK                0x47
#define KC_PAUSE                      0x48
#define KC_INSERT                     0x49
#define KC_HOME                       0x4A
#define KC_PAGE_UP                    0x4B
#define KC_DELETE                     0x4C
#define KC_END                        0x4D
#define KC_PAGE_DOWN                  0x4E
#define KC_ARROW_RIGHT                0x4F
#define KC_ARROW_LEFT                 0x50
#define KC_ARROW_DOWN                 0x51
#define KC_ARROW_UP                   0x52
#define KC_NUM_LOCK                   0x53
#define KC_KEYPAD_DIVIDE              0x54
#define KC_KEYPAD_MULTIPLY            0x55
#define KC_KEYPAD_SUBTRACT            0x56
#define KC_KEYPAD_ADD                 0x57
#define KC_KEYPAD_ENTER               0x58
#define KC_KEYPAD_1                   0x59
#define KC_KEYPAD_2                   0x5A
#define KC_KEYPAD_3                   0x5B
#define KC_KEYPAD_4                   0x5C
#define KC_KEYPAD_5                   0x5D
#define KC_KEYPAD_6                   0x5E
#define KC_KEYPAD_7                   0x5F
#define KC_KEYPAD_8                   0x60
#define KC_KEYPAD_9                   0x61
#define KC_KEYPAD_0                   0x62
#define KC_KEYPAD_PERIOD              0x63
#define KC_EUROPE_2                   0x64
#define KC_APPLICATION                0x65
#define KC_POWER                      0x66
#define KC_KEYPAD_EQUAL               0x67
#define KC_F13                        0x68
#define KC_F14                        0x69
#define KC_F15                        0x6A
#define KC_F16                        0x6B
#define KC_F17                        0x6C
#define KC_F18                        0x6D
#define KC_F19                        0x6E
#define KC_F20                        0x6F
#define KC_F21                        0x70
#define KC_F22                        0x71
#define KC_F23                        0x72
#define KC_F24                        0x73
#define KC_EXECUTE                    0x74
#define KC_HELP                       0x75
#define KC_MENU                       0x76
#define KC_SELECT                     0x77
#define KC_STOP                       0x78
#define KC_AGAIN                      0x79
#define KC_UNDO                       0x7A
#define KC_CUT                        0x7B
#define KC_COPY                       0x7C
#define KC_PASTE                      0x7D
#define KC_FIND                       0x7E
#define KC_MUTE                       0x7F
#define KC_VOLUME_UP                  0x80
#define KC_VOLUME_DOWN                0x81
#define KC_LOCKING_CAPS_LOCK          0x82
#define KC_LOCKING_NUM_LOCK           0x83
#define KC_LOCKING_SCROLL_LOCK        0x84
#define KC_KEYPAD_COMMA               0x85
#define KC_KEYPAD_EQUAL_SIGN          0x86
#define KC_KANJI1                     0x87
#define KC_KANJI2                     0x88
#define KC_KANJI3                     0x89
#define KC_KANJI4                     0x8A
#define KC_KANJI5                     0x8B
#define KC_KANJI6                     0x8C
#define KC_KANJI7                     0x8D
#define KC_KANJI8                     0x8E
#define KC_KANJI9                     0x8F
#define KC_LANG1                      0x90
#define KC_LANG2                      0x91
#define KC_LANG3                      0x92
#define KC_LANG4                      0x93
#define KC_LANG5                      0x94
#define KC_LANG6                      0x95
#define KC_LANG7                      0x96
#define KC_LANG8                      0x97
#define KC_LANG9                      0x98
#define KC_ALTERNATE_ERASE            0x99
#define KC_SYSREQ_ATTENTION           0x9A
#define KC_CANCEL                     0x9B
#define KC_CLEAR                      0x9C
#define KC_PRIOR                      0x9D
#define KC_RETURN                     0x9E
#define KC_SEPARATOR                  0x9F
#define KC_OUT                        0xA0
#define KC_OPER                       0xA1
#define KC_CLEAR_AGAIN                0xA2
#define KC_CRSEL_PROPS                0xA3
#define KC_EXSEL                      0xA4
// RESERVED					          0xA5 to 0xAF
#define KC_KEYPAD_00                  0xB0
#define KC_KEYPAD_000                 0xB1
#define KC_THOUSAND_SEPARATOR         0xB2
#define KC_DECIMAL_SEPARATOR          0xB3
#define KC_CURRENCY_UNIT              0xB4
#define KC_CURRENCY_SUBUNIT           0xB5
#define KC_KEYPAD_BRACKET_LEFT        0xB6
#define KC_KEYPAD_BRACKET_RIGHT       0xB7
#define KC_KEYPAD_CURLY_BRACKET_LEFT  0xB8
#define KC_KEYPAD_CURLY_BRACKET_RIGHT 0xB9
#define KC_KEYPAD_TAB                 0xBA
#define KC_KEYPAD_BACKSPACE           0xBB
#define KC_KEYPAD_A                   0xBC
#define KC_KEYPAD_B                   0xBD
#define KC_KEYPAD_C                   0xBE
#define KC_KEYPAD_D                   0xBF
#define KC_KEYPAD_E                   0xC0
#define KC_KEYPAD_F                   0xC1
#define KC_KEYPAD_XOR                 0xC2
#define KC_KEYPAD_CIRCUMFLEX          0xC3 // ^
#define KC_KEYPAD_PERCENT             0xC4
#define KC_KEYPAD_CHEVRON_LEFT        0xC5 // <
#define KC_KEYPAD_CHEVRON_RIGHT       0xC6 // >
#define KC_KEYPAD_AMPERSAND           0xC7 // &
#define KC_KEYPAD_AND                 0xC8 // &&
#define KC_KEYPAD_PIPE                0xC9 // |
#define KC_KEYPAD_OR                  0xCA // ||
#define KC_KEYPAD_COLON               0xCB // :
#define KC_KEYPAD_HASHTAG             0xCC
#define KC_KEYPAD_SPACE               0xCD
#define KC_KEYPAD_AT                  0xCE // @
#define KC_KEYPAD_EXCLAMATION_MARK    0xCF // !
#define KC_KEYPAD_MEMORY_STORE        0xD0
#define KC_KEYPAD_MEMORY_RECALL       0xD1
#define KC_KEYPAD_MEMORY_CLEAR        0xD2
#define KC_KEYPAD_MEMORY_ADD          0xD3
#define KC_KEYPAD_MEMORY_SUBTRACT     0XD4
#define KC_KEYPAD_MEMORY_MULTIPLY     0xD5
#define KC_KEYPAD_MEMORY_DIVIDE       0xD6
#define KC_KEYPAD_PLUS_OR_MINUS       0xD7
#define KC_KEYPAD_CLEAR               0xD8
#define KC_KEYPAD_CLEAR_ENTRY         0xD9
#define KC_KEYPAD_BINARY              0xDA
#define KC_KEYPAD_OCTAL               0xDB
#define KC_KEYPAD_DECIMAL             0xDC
#define KC_KEYPAD_HEXADECIMAL         0xDD
// RESERVED                           0xDE to 0xDF 
#define KC_CTRL_LEFT                  0xE0
#define KC_SHIFT_LEFT                 0xE1
#define KC_ALT_LEFT                   0xE2
#define KC_GUI_LEFT                   0xE3
#define KC_CTRL_RIGHT                 0xE4
#define KC_SHIFT_RIGHT                0xE5
#define KC_ALT_RIGHT                  0xE6
#define KC_GUI_RIGHT                  0xE7
// RESERVED                           0xE8 to 0xFFFF

// MODIFIERS
#define KC_MOD_NONE        0b00000000
#define KC_MOD_CTRL_LEFT   0b00000001
#define KC_MOD_SHIFT_LEFT  0b00000010
#define KC_MOD_ALT_LEFT    0b00000100
#define KC_MOD_GUI_LEFT    0b00001000
#define KC_MOD_CTRL_RIGHT  0b00010000
#define KC_MOD_SHIFT_RIGHT 0b00100000
#define KC_MOD_ALT_RIGHT   0b01000000
#define KC_MOD_GUI_RIGHT   0b10000000

// USB HID CONSUMER CONTROL
/**
 *  OOC -> On/Off Control
 *  OSC -> One Shot Control
 *  LC  -> Linear Control
 *  RTC -> Re-Trigger Control 
 */
#define KC_MEDIA_MUTE                 0x00E2
#define KC_MEDIA_VOLUME_INCREMENT     0x00E9
#define KC_MEDIA_VOLUME_DECREMENT     0x00EA
#define KC_MEDIA_PLAY_PAUSE           0x00CD

// USB HID LEDS
#define LED_UNDEFINED                 0x00
#define LED_NUM_LOCK                  0x01
#define LED_CAPS_LOCK                 0x02
#define LED_SCROLL_LOCK               0x03
#define LED_COMPOSE                   0x04
#define LED_KANA                      0x05
#define LED_POWER                     0x06
#define LED_SHIFT                     0x07
#define LED_DO_NOT_DISTURB            0x08
#define LED_MUTE                      0x09
#define LED_TONE_ENABLE               0x0A
#define LED_HIGH_CUT_FILTER           0x0B
#define LED_LOW_CUT_FILTER            0x0C
#define LED_EQUALIZER_ENABLE          0x0D
#define LED_SOUND_FIELD               0x0E
#define LED_SURROUND_ON               0x0F
#define LED_REPEAT                    0x10
#define LED_STEREO                    0x11
#define LED_SAMPLING_RATE_DETECT      0x12
#define LED_SPINNING                  0x13
#define LED_CAV                       0x14
#define LED_CLV                       0x15
#define LED_RECORDING_FORMAT_DETECT   0x16
#define LED_OFF_HOOK                  0x17
#define LED_RING                      0x18
#define LED_MESSAGE_WAITING           0x19
#define LED_DATA_MODE                 0x1A
#define LED_BATTERY_OPERATION         0x1B
#define LED_BATTERY_OK                0x1C
#define LED_BATTERY_LOW               0x1D
#define LED_SPEAKER                   0x1E
#define LED_HEADSET                   0x1F
#define LED_HOLD                      0x20
#define LED_MICROPHONE                0x21
#define LED_COVERAGE                  0x22
#define LED_NIGHT_MODE                0x23
#define LED_SEND_CALLS                0x24
#define LED_CALL_PICKUP               0x25
#define LED_CONFERENCE                0x26
#define LED_STAND_BY                  0x27
#define LED_CAMERA_ON                 0x28
#define LED_CAMERA_OFF                0x29
#define LED_ON_LINE                   0x2A
#define LED_OFF_LINE                  0x2B
#define LED_BUSY                      0x2C
#define LED_READY                     0x2D
#define LED_PAPER_OUT                 0x2E
#define LED_PAPER_JAM                 0x2F
#define LED_REMOTE                    0x30
#define LED_FORWARD                   0x31
#define LED_REVERSE                   0x32
#define LED_STOP                      0x33
#define LED_REWIND                    0x34
#define LED_FAST_FORWARD              0x35
#define LED_PLAY                      0x36
#define LED_PAUSE                     0x37
#define LED_RECORD                    0x38
#define LED_ERROR                     0x39
#define LED_USAGE_SELECTED_INDICATOR  0x3A
#define LED_USAGE_IN_USE_INDICATOR    0x3B
#define LED_USAGE_MULTI_MODE_INDICATOR 0x3C
#define LED_INDICATOR_ON              0x3D
#define LED_INDICATOR_FLASH           0x3E
#define LED_INDICATOR_SLOW_BLINK      0x3F
#define LED_INDICATOR_FAST_BLINK      0x40
#define LED_INDICATOR_OFF             0x41
#define LED_FLASH_ON_TIME             0x42
#define LED_SLOW_BLINK_ON_TIME        0x43
#define LED_SLOW_BLINK_OFF_TIME       0x44
#define LED_FAST_BLINK_ON_TIME        0x45
#define LED_FAST_BLINK_OFF_TIME       0x46
#define LED_USAGE_INDICATOR_COLOR     0x47
#define LED_INDICATOR_RED             0x48
#define LED_INDICATOR_GREEN           0x49
#define LED_INDICATOR_AMBER           0x4A
#define LED_GENERIC_INDICATOR         0x4B
#define LED_SYSTEM_SUSPEND            0x4C
#define LED_EXTERNAL_POWER_CONNECTED  0x4D
#define LED_INDICATOR_BLUE            0x4E
#define LED_INDICATOR_ORANGE          0x4F
#define LED_GOOD_STATUS               0x50
#define LED_WARNING_STATUS            0x51
#define LED_RGB_LED                   0x52
#define LED_RED_LED_CHANNEL           0x53
#define LED_BLUE_LED_CHANNEL          0x54
#define LED_GREEN_LED_CHANNEL         0x55
#define LED_LED_INTENSITY             0x56
#define LED_SYSTEM_MICROPHONE_MUTE    0x57
// RESERVED                           0x5F to 0x58
#define LED_PLAYER_INDICATOR          0x60
#define LED_PLAYER_1                  0x61
#define LED_PLAYER_2                  0x62
#define LED_PLAYER_3                  0x63
#define LED_PLAYER_4                  0x64
#define LED_PLAYER_5                  0x65
#define LED_PLAYER_6                  0x66
#define LED_PLAYER_7                  0x67
#define LED_PLAYER_8                  0x68
// RESERVED                           0x69 to 0xFFF

#define GAMEPAD_BUTTON_A 1 << 0
#define GAMEPAD_BUTTON_B 1 << 1
#define GAMEPAD_BUTTON_C 1 << 2
#define GAMEPAD_BUTTON_X 1 << 3
#define GAMEPAD_BUTTON_Y 1 << 4
#define GAMEPAD_BUTTON_Z 1 << 5

#define GAMEPAD_BUTTON_LT 1 << 6
#define GAMEPAD_BUTTON_RT 1 << 7
#define GAMEPAD_BUTTON_LB 1 << 8
#define GAMEPAD_BUTTON_RB 1 << 9

#define GAMEPAD_BUTTON_SELECT 1 << 10
#define GAMEPAD_BUTTON_START 1 << 11

#endif
