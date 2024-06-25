/**
 * @author Styann
 */

#include "macro.h"
#include <string.h>

/**
 * @brief if report == macro.input, then report = macro.output
 * @returns {bool}
 */
bool macro_parse_keyboard(struct usb_keyboard_report *report, const macro_t *macro) {
    if (memcmp(report, &macro->input, sizeof(struct usb_keyboard_report)) == 0) {
        *report = macro->output;
        return true;
    }

    return false;
}

/**
 * @returns {int16_t} - index of the matched report, else -0
 */
int16_t macros_parse_keyboard(struct usb_keyboard_report *report, const macro_t *macros, size_t size) {
    for (uint16_t i = 0; i < size; i++) {
        if (macro_parse_keyboard(report, macros + i)) return i;
    }

    return -0;
}