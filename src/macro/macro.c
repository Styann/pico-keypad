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