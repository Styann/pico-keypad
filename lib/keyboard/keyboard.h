/**
 * @author Styann
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

typedef struct keyboard_matrix {
    const uint8_t *layout;
    const uint8_t *rows_pins;
    const uint8_t *columns_pins;
    const uint16_t row_size;
    const uint16_t column_size;
} keyboard_matrix_t;

void keyboard_matrix_init(keyboard_matrix_t *this);

static bool is_key_pressed(uint8_t column_pin);

void keyboard_matrix_scan(keyboard_matrix_t *this, struct usb_keyboard_report *report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

static bool try_add_modifier(struct usb_keyboard_report *report, uint8_t keycode);

static void push_keycode(struct usb_keyboard_report *report, uint8_t keycode);

bool is_key_in_report(const struct usb_keyboard_report *report, const uint8_t keycode);

bool is_keyboard_report_empty(const struct usb_keyboard_report *report);

bool keyboard_report_cmp(const struct usb_keyboard_report *x, const struct usb_keyboard_report *y);

bool is_gamepad_report_empty(struct usb_gamepad_report *report);

#endif
