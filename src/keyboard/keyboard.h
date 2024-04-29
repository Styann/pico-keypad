#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

void keyboard_init(void);

static bool is_key_pressed(uint8_t column_pin);

void scan_keyboard(struct usb_keyboard_report *keyboard_report);

static void set_keyboard_report(struct usb_keyboard_report *keyboard_report, uint8_t keycode);

bool assert_keyboard_reports(struct usb_keyboard_report *dest, struct usb_keyboard_report *src);

bool is_keyboard_report_empty(struct usb_keyboard_report *keyboard_report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

void release(void);

void send_consumer_control(uint16_t consumer_control);

#endif