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

static void add_keycode(struct usb_keyboard_report *report, uint8_t keycode);

bool has_keyboard_report_changed(struct usb_keyboard_report *dest, struct usb_keyboard_report *src);

bool is_keyboard_report_empty(struct usb_keyboard_report *report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

void usb_send_keyboard_report(struct usb_keyboard_report *report);

void usb_send_consumer_control(uint16_t consumer_control);

void release_keyboard(void);

void usb_send_gamepad_report(struct usb_gamepad_report *report);

bool is_gamepad_report_empty(struct usb_gamepad_report *report);

#endif