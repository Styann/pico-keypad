/**
 * @author Styann
 */

#include "keyboard.h"
#include <string.h>
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "../pico_extra/pico_extra.h"
#include "../usb/usb.h"

#define KRO 6 // number of keycodes in usb keyboard report

/**
 * @brief Set all rows pins as OUPUT and HIGH then all columns pins as GPIO INPUT PULL UP
 */
void keyboard_matrix_init(keyboard_matrix_t *self) {
    for (uint16_t r = 0; r < self->row_length; r++) {
        gpio_init(self->rows_pins[r]);
        gpio_set_dir(self->rows_pins[r], GPIO_OUT);
        gpio_put(self->rows_pins[r], HIGH);
    }

    for (uint16_t c = 0; c < self->column_length; c++) {
        gpio_init(self->columns_pins[c]);
        gpio_set_dir(self->columns_pins[c], GPIO_IN);
        gpio_pull_up(self->columns_pins[c]);
    }
}

/**
 * @brief return whether a key is pressed or not
 * @param column_pin
 */
static bool is_key_pressed(uint8_t column_pin) {
    return !gpio_get(column_pin);
};

/**
 * @brief loop through the matrix and add pressed keys to a keyboard report
 * @param matrix
 * @param report
 */
void keyboard_matrix_scan(keyboard_matrix_t *self, struct usb_keyboard_report *report) {
    uint8_t pressed_keys_count = 0;

    for (uint8_t r = 0; pressed_keys_count < KRO && r < self->row_length; r++) {
        gpio_put(self->rows_pins[r], LOW); // low output pin act as a ground
        busy_wait_us_32(10); // I fucking broke one pcb for this shit

        for (uint8_t c = 0; pressed_keys_count < KRO && c < self->column_length; c++) {
            uint8_t keycode = self->layout[r * self->column_length + c];

            if (is_key_pressed(self->columns_pins[c]) && keycode != KC_NONE) {
                if (keycode >= KC_CTRL_LEFT && keycode <= KC_GUI_RIGHT) {
                    report->modifiers |= get_modifier_from_keycode(keycode);
                }
                else {
                    push_keycode(report, keycode);
                    pressed_keys_count++;
                }
            }
        }

        gpio_put(self->rows_pins[r], HIGH);
    }
}

/**
 * @brief take a keycode and return his modifier if he has one
 * @param keycode must be between KC_CTRL_LEFT (0xE0) and KC_GUI_RIGHT (0xE7)
 */
static uint8_t get_modifier_from_keycode(uint8_t keycode) {
    return (0x01 << (keycode & 0b00001111));
}

/**
 * @brief push it into keycodes array
 * @param report
 * @param keycode
 */
static void push_keycode(struct usb_keyboard_report *report, uint8_t keycode) {
     for (uint8_t i = 0; i < KRO; i++) {
        if (report->keycodes[i] == KC_NONE) {
            report->keycodes[i] = keycode;
            break;
        }
    }
}

bool is_key_in_report(const struct usb_keyboard_report *report, const uint8_t keycode) {
    for (uint8_t i = 0; i < KRO; i++) {
        if (report->keycodes[i] == keycode) return true;
    }

    return false;
}

/**
 * @brief return true if modifiers and keycodes are set to 0, else false
 * @param keyboard_report
 */
bool is_keyboard_report_empty(const struct usb_keyboard_report *report) {
    if (report->modifiers != MOD_NONE) return false;

    for (uint8_t i = 0; i < KRO; i++) {
        if (report->keycodes[i] != KC_NONE) return false;
    }

    return true;
}

bool keyboard_report_cmp(const struct usb_keyboard_report *x, const struct usb_keyboard_report *y) {
    return (memcmp(x, y, sizeof(struct usb_keyboard_report)) == 0) ? true : false;
}

bool is_gamepad_report_empty(struct usb_gamepad_report *report) {
    return !(report->x | report->y | report->buttons);
}
