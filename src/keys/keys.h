#ifndef KEYS_H
#define KEYS_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

void keys_init(void);

static bool is_key_pressed(uint8_t column_pin);

static inline void set_pin_output_write_low(uint8_t pin);

static inline void set_pin_output_write_high(uint8_t pin);

void scan_keyboard(struct usb_hid_keyboard_report *keyboard_report);

static void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, uint8_t keycode);

bool assert_keyboard_reports(struct usb_hid_keyboard_report *dest, struct usb_hid_keyboard_report *src);

bool is_keyboard_report_empty(struct usb_hid_keyboard_report *keyboard_report);

static uint8_t get_modifier_from_keycode(uint8_t keycode);

void release(void);

#endif