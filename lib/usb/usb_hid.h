#ifndef USB_HID_H
#define USB_HID_H

#include "pico/stdlib.h"

#define EP_IN_HID 0x81

struct usb_gamepad_report {
    // uint8_t id;
    uint8_t x;
    uint8_t y;
    uint8_t buttons;
} __packed;

struct usb_mouse_report {
    uint8_t id;
    uint8_t buttons;
    int16_t  x;
    int16_t  y;
    int8_t  wheel;
    // int8_t  pan;
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

void usb_send_keyboard_report(const struct usb_keyboard_report *report);

void usb_send_mouse_report(const struct usb_mouse_report *report);

void usb_send_single_keycode(uint8_t keycode);

void release_keyboard(void);

void usb_send_consumer_control_report(struct usb_consumer_control_report *report);

void usb_send_consumer_control(uint16_t consumer_control);

void usb_send_gamepad_report(struct usb_gamepad_report *report);

enum usb_keycodes {
    KC_NONE                       = 0x00,
    KC_ERROR_ROLL_OVER            = 0x01,
    KC_POSTFAIL                   = 0x02,
    KC_ERROR_UNDEFINED            = 0x03,
    KC_A                          = 0x04,
    KC_B                          = 0x05,
    KC_C                          = 0x06,
    KC_D                          = 0x07,
    KC_E                          = 0x08,
    KC_F                          = 0x09,
    KC_G                          = 0x0A,
    KC_H                          = 0x0B,
    KC_I                          = 0x0C,
    KC_J                          = 0x0D,
    KC_K                          = 0x0E,
    KC_L                          = 0x0F,
    KC_M                          = 0x10,
    KC_N                          = 0x11,
    KC_O                          = 0x12,
    KC_P                          = 0x13,
    KC_Q                          = 0x14,
    KC_R                          = 0x15,
    KC_S                          = 0x16,
    KC_T                          = 0x17,
    KC_U                          = 0x18,
    KC_V                          = 0x19,
    KC_W                          = 0x1A,
    KC_X                          = 0x1B,
    KC_Y                          = 0x1C,
    KC_Z                          = 0x1D,
    KC_1                          = 0x1E,
    KC_2                          = 0x1F,
    KC_3                          = 0x20,
    KC_4                          = 0x21,
    KC_5                          = 0x22,
    KC_6                          = 0x23,
    KC_7                          = 0x24,
    KC_8                          = 0x25,
    KC_9                          = 0x26,
    KC_0                          = 0x27,
    KC_ENTER                      = 0x28,
    KC_ESCAPE                     = 0x29,
    KC_BACKSPACE                  = 0x2A,
    KC_TAB                        = 0x2B,
    KC_SPACE                      = 0x2C,
    KC_MINUS                      = 0x2D, // -
    KC_EQUAL                      = 0x2E, // =
    KC_SQUARE_BRACKET_LEFT        = 0x2F, // { [
    KC_SQUARE_BRACKET_RIGHT       = 0x30, // } ]
    KC_BACKSLASH                  = 0x31,
    KC_EUROPE_1                   = 0x32,
    KC_SEMICOLON                  = 0x33, // ; :
    KC_APOSTROPHE                 = 0x34,
    KC_GRAVE                      = 0x35, // ` ~
    KC_COMMA                      = 0x36, // , <
    KC_PERIOD                     = 0x37, // . >
    KC_SLASH                      = 0x38,
    KC_CAPS_LOCK                  = 0x39,
    KC_F1                         = 0x3A,
    KC_F2                         = 0x3B,
    KC_F3                         = 0x3C,
    KC_F4                         = 0x3D,
    KC_F5                         = 0x3E,
    KC_F6                         = 0x3F,
    KC_F7                         = 0x40,
    KC_F8                         = 0x41,
    KC_F9                         = 0x42,
    KC_F10                        = 0x43,
    KC_F11                        = 0x44,
    KC_F12                        = 0x45,
    KC_PRINT_SCREEN               = 0x46,
    KC_SCROLL_LOCK                = 0x47,
    KC_PAUSE                      = 0x48,
    KC_INSERT                     = 0x49,
    KC_HOME                       = 0x4A,
    KC_PAGE_UP                    = 0x4B,
    KC_DELETE                     = 0x4C,
    KC_END                        = 0x4D,
    KC_PAGE_DOWN                  = 0x4E,
    KC_ARROW_RIGHT                = 0x4F,
    KC_ARROW_LEFT                 = 0x50,
    KC_ARROW_DOWN                 = 0x51,
    KC_ARROW_UP                   = 0x52,
    KC_NUM_LOCK                   = 0x53,
    KC_KEYPAD_DIVIDE              = 0x54,
    KC_KEYPAD_MULTIPLY            = 0x55,
    KC_KEYPAD_SUBTRACT            = 0x56,
    KC_KEYPAD_ADD                 = 0x57,
    KC_KEYPAD_ENTER               = 0x58,
    KC_KEYPAD_1                   = 0x59,
    KC_KEYPAD_2                   = 0x5A,
    KC_KEYPAD_3                   = 0x5B,
    KC_KEYPAD_4                   = 0x5C,
    KC_KEYPAD_5                   = 0x5D,
    KC_KEYPAD_6                   = 0x5E,
    KC_KEYPAD_7                   = 0x5F,
    KC_KEYPAD_8                   = 0x60,
    KC_KEYPAD_9                   = 0x61,
    KC_KEYPAD_0                   = 0x62,
    KC_KEYPAD_PERIOD              = 0x63,
    KC_EUROPE_2                   = 0x64,
    KC_APPLICATION                = 0x65,
    KC_POWER                      = 0x66,
    KC_KEYPAD_EQUAL               = 0x67,
    KC_F13                        = 0x68,
    KC_F14                        = 0x69,
    KC_F15                        = 0x6A,
    KC_F16                        = 0x6B,
    KC_F17                        = 0x6C,
    KC_F18                        = 0x6D,
    KC_F19                        = 0x6E,
    KC_F20                        = 0x6F,
    KC_F21                        = 0x70,
    KC_F22                        = 0x71,
    KC_F23                        = 0x72,
    KC_F24                        = 0x73,
    KC_EXECUTE                    = 0x74,
    KC_HELP                       = 0x75,
    KC_MENU                       = 0x76,
    KC_SELECT                     = 0x77,
    KC_STOP                       = 0x78,
    KC_AGAIN                      = 0x79,
    KC_UNDO                       = 0x7A,
    KC_CUT                        = 0x7B,
    KC_COPY                       = 0x7C,
    KC_PASTE                      = 0x7D,
    KC_FIND                       = 0x7E,
    KC_MUTE                       = 0x7F,
    KC_VOLUME_UP                  = 0x80,
    KC_VOLUME_DOWN                = 0x81,
    KC_LOCKING_CAPS_LOCK          = 0x82,
    KC_LOCKING_NUM_LOCK           = 0x83,
    KC_LOCKING_SCROLL_LOCK        = 0x84,
    KC_KEYPAD_COMMA               = 0x85,
    KC_KEYPAD_EQUAL_SIGN          = 0x86,
    KC_KANJI1                     = 0x87,
    KC_KANJI2                     = 0x88,
    KC_KANJI3                     = 0x89,
    KC_KANJI4                     = 0x8A,
    KC_KANJI5                     = 0x8B,
    KC_KANJI6                     = 0x8C,
    KC_KANJI7                     = 0x8D,
    KC_KANJI8                     = 0x8E,
    KC_KANJI9                     = 0x8F,
    KC_LANG1                      = 0x90,
    KC_LANG2                      = 0x91,
    KC_LANG3                      = 0x92,
    KC_LANG4                      = 0x93,
    KC_LANG5                      = 0x94,
    KC_LANG6                      = 0x95,
    KC_LANG7                      = 0x96,
    KC_LANG8                      = 0x97,
    KC_LANG9                      = 0x98,
    KC_ALTERNATE_ERASE            = 0x99,
    KC_SYSREQ_ATTENTION           = 0x9A,
    KC_CANCEL                     = 0x9B,
    KC_CLEAR                      = 0x9C,
    KC_PRIOR                      = 0x9D,
    KC_RETURN                     = 0x9E,
    KC_SEPARATOR                  = 0x9F,
    KC_OUT                        = 0xA0,
    KC_OPER                       = 0xA1,
    KC_CLEAR_AGAIN                = 0xA2,
    KC_CRSEL_PROPS                = 0xA3,
    KC_EXSEL                      = 0xA4,
    // RESERVED					    0xA5 to 0xAF
    KC_KEYPAD_00                  = 0xB0,
    KC_KEYPAD_000                 = 0xB1,
    KC_THOUSAND_SEPARATOR         = 0xB2,
    KC_DECIMAL_SEPARATOR          = 0xB3,
    KC_CURRENCY_UNIT              = 0xB4,
    KC_CURRENCY_SUBUNIT           = 0xB5,
    KC_KEYPAD_BRACKET_LEFT        = 0xB6,
    KC_KEYPAD_BRACKET_RIGHT       = 0xB7,
    KC_KEYPAD_CURLY_BRACKET_LEFT  = 0xB8,
    KC_KEYPAD_CURLY_BRACKET_RIGHT = 0xB9,
    KC_KEYPAD_TAB                 = 0xBA,
    KC_KEYPAD_BACKSPACE           = 0xBB,
    KC_KEYPAD_A                   = 0xBC,
    KC_KEYPAD_B                   = 0xBD,
    KC_KEYPAD_C                   = 0xBE,
    KC_KEYPAD_D                   = 0xBF,
    KC_KEYPAD_E                   = 0xC0,
    KC_KEYPAD_F                   = 0xC1,
    KC_KEYPAD_XOR                 = 0xC2,
    KC_KEYPAD_CIRCUMFLEX          = 0xC3, // ^
    KC_KEYPAD_PERCENT             = 0xC4,
    KC_KEYPAD_CHEVRON_LEFT        = 0xC5, // <
    KC_KEYPAD_CHEVRON_RIGHT       = 0xC6, // >
    KC_KEYPAD_AMPERSAND           = 0xC7, // &
    KC_KEYPAD_AND                 = 0xC8, // &&
    KC_KEYPAD_PIPE                = 0xC9, // |
    KC_KEYPAD_OR                  = 0xCA, // ||
    KC_KEYPAD_COLON               = 0xCB, // :
    KC_KEYPAD_HASHTAG             = 0xCC,
    KC_KEYPAD_SPACE               = 0xCD,
    KC_KEYPAD_AT                  = 0xCE, // @
    KC_KEYPAD_EXCLAMATION_MARK    = 0xCF, // !
    KC_KEYPAD_MEMORY_STORE        = 0xD0,
    KC_KEYPAD_MEMORY_RECALL       = 0xD1,
    KC_KEYPAD_MEMORY_CLEAR        = 0xD2,
    KC_KEYPAD_MEMORY_ADD          = 0xD3,
    KC_KEYPAD_MEMORY_SUBTRACT     = 0XD4,
    KC_KEYPAD_MEMORY_MULTIPLY     = 0xD5,
    KC_KEYPAD_MEMORY_DIVIDE       = 0xD6,
    KC_KEYPAD_PLUS_OR_MINUS       = 0xD7,
    KC_KEYPAD_CLEAR               = 0xD8,
    KC_KEYPAD_CLEAR_ENTRY         = 0xD9,
    KC_KEYPAD_BINARY              = 0xDA,
    KC_KEYPAD_OCTAL               = 0xDB,
    KC_KEYPAD_DECIMAL             = 0xDC,
    KC_KEYPAD_HEXADECIMAL         = 0xDD,
    // RESERVED                     0xDE to 0xDF
    KC_CTRL_LEFT                  = 0xE0,
    KC_SHIFT_LEFT                 = 0xE1,
    KC_ALT_LEFT                   = 0xE2,
    KC_GUI_LEFT                   = 0xE3,
    KC_CTRL_RIGHT                 = 0xE4,
    KC_SHIFT_RIGHT                = 0xE5,
    KC_ALT_RIGHT                  = 0xE6,
    KC_GUI_RIGHT                  = 0xE7
    // RESERVED                     0xE8 to 0xFFFF
};

enum usb_keyboard_modifiers {
    MOD_NONE        = 0b00000000,
    MOD_CTRL_LEFT   = 0b00000001,
    MOD_SHIFT_LEFT  = 0b00000010,
    MOD_ALT_LEFT    = 0b00000100,
    MOD_GUI_LEFT    = 0b00001000,
    MOD_CTRL_RIGHT  = 0b00010000,
    MOD_SHIFT_RIGHT = 0b00100000,
    MOD_ALT_RIGHT   = 0b01000000,
    MOD_GUI_RIGHT   = 0b10000000
};

/**
 *  OOC -> On/Off Control
 *  OSC -> One Shot Control
 *  LC  -> Linear Control
 *  RTC -> Re-Trigger Control
 */
enum usb_consumer_controls {
    CC_BRIGHTNESS_INCREMENT = 0x006F,
    CC_BRIGHTNESS_DECREMENT = 0x0070,
    CC_NEXT_TRACK           = 0x00B5,
    CC_PREVIOUS_TRACK       = 0x00B6,
    CC_RANDOM_PLAY          = 0x00B9,
    CC_PLAY_PAUSE           = 0x00CD,
    CC_MUTE                 = 0x00E2,
    CC_VOLUME_INCREMENT     = 0x00E9,
    CC_VOLUME_DECREMENT     = 0x00EA,
    CC_RECORD               = 0x00B2
};

enum usb_generic_gui_app_control {
    CC_SCROLL_UP            = 0x0233,
    CC_SCROLL_DOWN          = 0x0234
};

enum usb_leds {
    LED_UNDEFINED                 = 0x00,
    LED_NUM_LOCK                  = 0x01,
    LED_CAPS_LOCK                 = 0x02,
    LED_SCROLL_LOCK               = 0x03,
    LED_COMPOSE                   = 0x04,
    LED_KANA                      = 0x05,
    LED_POWER                     = 0x06,
    LED_SHIFT                     = 0x07,
    LED_DO_NOT_DISTURB            = 0x08,
    LED_MUTE                      = 0x09,
    LED_TONE_ENABLE               = 0x0A,
    LED_HIGH_CUT_FILTER           = 0x0B,
    LED_LOW_CUT_FILTER            = 0x0C,
    LED_EQUALIZER_ENABLE          = 0x0D,
    LED_SOUND_FIELD               = 0x0E,
    LED_SURROUND_ON               = 0x0F,
    LED_REPEAT                    = 0x10,
    LED_STEREO                    = 0x11,
    LED_SAMPLING_RATE_DETECT      = 0x12,
    LED_SPINNING                  = 0x13,
    LED_CAV                       = 0x14,
    LED_CLV                       = 0x15,
    LED_RECORDING_FORMAT_DETECT   = 0x16,
    LED_OFF_HOOK                  = 0x17,
    LED_RING                      = 0x18,
    LED_MESSAGE_WAITING           = 0x19,
    LED_DATA_MODE                 = 0x1A,
    LED_BATTERY_OPERATION         = 0x1B,
    LED_BATTERY_OK                = 0x1C,
    LED_BATTERY_LOW               = 0x1D,
    LED_SPEAKER                   = 0x1E,
    LED_HEADSET                   = 0x1F,
    LED_HOLD                      = 0x20,
    LED_MICROPHONE                = 0x21,
    LED_COVERAGE                  = 0x22,
    LED_NIGHT_MODE                = 0x23,
    LED_SEND_CALLS                = 0x24,
    LED_CALL_PICKUP               = 0x25,
    LED_CONFERENCE                = 0x26,
    LED_STAND_BY                  = 0x27,
    LED_CAMERA_ON                 = 0x28,
    LED_CAMERA_OFF                = 0x29,
    LED_ON_LINE                   = 0x2A,
    LED_OFF_LINE                  = 0x2B,
    LED_BUSY                      = 0x2C,
    LED_READY                     = 0x2D,
    LED_PAPER_OUT                 = 0x2E,
    LED_PAPER_JAM                 = 0x2F,
    LED_REMOTE                    = 0x30,
    LED_FORWARD                   = 0x31,
    LED_REVERSE                   = 0x32,
    LED_STOP                      = 0x33,
    LED_REWIND                    = 0x34,
    LED_FAST_FORWARD              = 0x35,
    LED_PLAY                      = 0x36,
    LED_PAUSE                     = 0x37,
    LED_RECORD                    = 0x38,
    LED_ERROR                     = 0x39,
    LED_USAGE_SELECTED_INDICATOR  = 0x3A,
    LED_USAGE_IN_USE_INDICATOR    = 0x3B,
    LED_USAGE_MULTI_MODE_INDICATOR = 0x3C,
    LED_INDICATOR_ON              = 0x3D,
    LED_INDICATOR_FLASH           = 0x3E,
    LED_INDICATOR_SLOW_BLINK      = 0x3F,
    LED_INDICATOR_FAST_BLINK      = 0x40,
    LED_INDICATOR_OFF             = 0x41,
    LED_FLASH_ON_TIME             = 0x42,
    LED_SLOW_BLINK_ON_TIME        = 0x43,
    LED_SLOW_BLINK_OFF_TIME       = 0x44,
    LED_FAST_BLINK_ON_TIME        = 0x45,
    LED_FAST_BLINK_OFF_TIME       = 0x46,
    LED_USAGE_INDICATOR_COLOR     = 0x47,
    LED_INDICATOR_RED             = 0x48,
    LED_INDICATOR_GREEN           = 0x49,
    LED_INDICATOR_AMBER           = 0x4A,
    LED_GENERIC_INDICATOR         = 0x4B,
    LED_SYSTEM_SUSPEND            = 0x4C,
    LED_EXTERNAL_POWER_CONNECTED  = 0x4D,
    LED_INDICATOR_BLUE            = 0x4E,
    LED_INDICATOR_ORANGE          = 0x4F,
    LED_GOOD_STATUS               = 0x50,
    LED_WARNING_STATUS            = 0x51,
    LED_RGB_LED                   = 0x52,
    LED_RED_LED_CHANNEL           = 0x53,
    LED_BLUE_LED_CHANNEL          = 0x54,
    LED_GREEN_LED_CHANNEL         = 0x55,
    LED_LED_INTENSITY             = 0x56,
    LED_SYSTEM_MICROPHONE_MUTE    = 0x57,
    // RESERVED                     0x5F to 0x58
    LED_PLAYER_INDICATOR          = 0x60,
    LED_PLAYER_1                  = 0x61,
    LED_PLAYER_2                  = 0x62,
    LED_PLAYER_3                  = 0x63,
    LED_PLAYER_4                  = 0x64,
    LED_PLAYER_5                  = 0x65,
    LED_PLAYER_6                  = 0x66,
    LED_PLAYER_7                  = 0x67,
    LED_PLAYER_8                  = 0x68
    // RESERVED                     0x69 to 0xFFF
};

enum usb_gamepad_buttons {
    GAMEPAD_BUTTON_A = 1 << 0,
    GAMEPAD_BUTTON_B = 1 << 1,
    GAMEPAD_BUTTON_C = 1 << 2,
    GAMEPAD_BUTTON_X = 1 << 3,
    GAMEPAD_BUTTON_Y = 1 << 4,
    GAMEPAD_BUTTON_Z = 1 << 5,
    GAMEPAD_BUTTON_LT = 1 << 6,
    GAMEPAD_BUTTON_RT = 1 << 7,
    GAMEPAD_BUTTON_LB = 1 << 8,
    GAMEPAD_BUTTON_RB = 1 << 9,
    GAMEPAD_BUTTON_SELECT = 1 << 10,
    GAMEPAD_BUTTON_START = 1 << 11
};

#endif
