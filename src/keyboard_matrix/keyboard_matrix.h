#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

void keyboard_matrix_init(void);

static bool is_key_pressed(uint8_t column_pin);

void keyboard_matrix_scan(struct usb_keyboard_report *report);

static void set_keyboard_report(struct usb_keyboard_report *report, uint8_t keycode);

bool has_keyboard_report_changed(struct usb_keyboard_report *dest, struct usb_keyboard_report *src);

bool is_keyboard_report_empty(struct usb_keyboard_report *report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

void usb_send_keyboard_report(struct usb_keyboard_report *report);

void usb_send_consumer_control(uint16_t consumer_control);

void release_keyboard(void);

void usb_send_gamepad_report(struct usb_gamepad_report *report);

#endif