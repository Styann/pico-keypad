#ifndef KEYS_H
#define KEYS_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5

void keys_init(void);

static bool is_key_pressed(uint8_t column_pin);

static inline void set_pin_output_write_low(uint8_t pin);

static inline void set_pin_output_write_high(uint8_t pin);

void scan_keyboard(struct usb_hid_keyboard_report *keyboard_report);

static void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, uint8_t keycode);

bool assert_keyboard_reports(struct usb_hid_keyboard_report *dest, struct usb_hid_keyboard_report *src);

bool is_keyboard_report_empty(struct usb_hid_keyboard_report *keyboard_report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = { 2, 3, 4, 5 };
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = { 11, 12, 13, 14, 15 };

static uint8_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    { KC_NUM_LOCK ,     KC_KEYPAD_DIVIDE, KC_KEYPAD_MULTIPLY, KC_KEYPAD_SUBTRACT },
    { KC_KEYPAD_7,      KC_KEYPAD_8,      KC_KEYPAD_9,        KC_KEYPAD_ADD      },
    { KC_KEYPAD_4,      KC_KEYPAD_5,      KC_KEYPAD_6,        KC_KEYPAD_ENTER    },
    { KC_KEYPAD_1,      KC_KEYPAD_2,      KC_KEYPAD_3,        KC_KEYPAD_0        },
    { KC_KEYPAD_PERIOD, KC_NONE,          KC_NONE,            KC_NONE            }
};

#endif