/**
 * @author Styann
 */

#ifndef MACRO_H
#define MACRO_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

enum fn_macro_types {
    FN_NONE,
    FN_CC, // consumer control
    FN_HL // function handler
};

typedef struct {
    const struct usb_keyboard_report input;
    const struct usb_keyboard_report output;
} macro_io_t;

typedef struct {
    const struct usb_keyboard_report input;
    void (*event_callback)(void);
} macro_input_event_t;

bool handle_macro_io(struct usb_keyboard_report *report, const macro_io_t *macro);

int16_t handle_macros_io(struct usb_keyboard_report *report, const macro_io_t *macros, size_t size);

bool handle_macro_input_event(const struct usb_keyboard_report *report, const macro_input_event_t *macro);

#endif
