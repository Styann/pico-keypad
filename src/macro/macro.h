/**
 * @author Styann
 */

#ifndef MACRO_H
#define MACRO_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

typedef struct {
    const struct usb_keyboard_report input;
    const struct usb_keyboard_report output;
} macro_t;

bool macro_parse_keyboard(struct usb_keyboard_report *report, const macro_t *macro);

#endif
