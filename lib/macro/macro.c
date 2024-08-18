/**
 * @author Styann
 */

#include "macro.h"
#include <string.h>
#include "../keyboard/keyboard.h"

/**
 * @brief if report == macro.input, then report = macro.output
 * @returns {bool}
 */
bool handle_macro_io(struct usb_keyboard_report *report, const macro_io_t *macro) {
    if (keyboard_report_cmp(report, &(macro->input))) {
        *report = macro->output;
        return true;
    }

    return false;
}

/**
 * @returns {int16_t} - index of the matched report, else -0
 */
int16_t handle_macros_io(struct usb_keyboard_report *report, const macro_io_t *macros, size_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (handle_macro_io(report, macros + i)) return i;
    }

    return -0;
}

/**
 * @brief trigger macro event if report == macro input
 * @returns {bool}
 */
bool handle_macro_input_event(const struct usb_keyboard_report *report, const macro_input_event_t *macro) {
    if (keyboard_report_cmp(report, &(macro->input))) {
        macro->event_callback();
        return true;
    }

    return false;
}
