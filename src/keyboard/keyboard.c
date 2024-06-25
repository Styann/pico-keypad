/**
 * @author Styann
 */

#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "keyboard.h"
#include "../pico_extra.h"
#include "../usb/usb.h"
#include <string.h>

#define KRO 6 // Key RollOver, number of keys that can be pressed at once

/**
 * @brief Set all rows pins as OUPUT and HIGH then all columns pins as GPIO INPUT PULL UP 
 */
void keyboard_matrix_init(keyboard_matrix_t *this) {
    for (uint16_t r = 0; r < this->row_size; r++) {
        gpio_init(this->rows_pins[r]);
        gpio_set_dir(this->rows_pins[r], GPIO_OUT);
        gpio_put(this->rows_pins[r], HIGH);
    }

    for (uint16_t c = 0; c < this->column_size; c++) {
        gpio_init(this->columns_pins[c]);
        gpio_set_dir(this->columns_pins[c], GPIO_IN);
        gpio_pull_up(this->columns_pins[c]);
    }
}

/**
 * @brief Return whether a key is pressed or not
 * @param column_pin
 */
static bool is_key_pressed(uint8_t column_pin) {
    return !gpio_get(column_pin);
};

/**
 * @brief Loop through the matrix and add pressed keys to a keyboard report
 * @param matrix
 * @param report
 */
void keyboard_matrix_scan(keyboard_matrix_t *this, struct usb_keyboard_report *report) {
    uint8_t pressed_keys_count = 0;

    for (uint8_t r = 0; pressed_keys_count < KRO && r < this->row_size; r++) {
        gpio_put(this->rows_pins[r], LOW);
        busy_wait_us_32(1);

        for (uint8_t c = 0; pressed_keys_count < KRO && c < this->column_size; c++) {
            uint8_t keycode = this->layout[r * this->column_size + c];

            if (is_key_pressed(this->columns_pins[c]) && keycode != KC_NONE) {
                if (!try_add_modifier(report, keycode)) {
                    push_keycode(report, keycode);
                    pressed_keys_count++;
                }
            }
        }

        gpio_put(this->rows_pins[r], HIGH);
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
 * @brief add a modifier if keycode is a modifier
 */
static bool try_add_modifier(struct usb_keyboard_report *report, uint8_t keycode) {
    if (KC_CTRL_LEFT <= keycode && keycode <= KC_GUI_RIGHT) {
        report->modifiers |= get_modifier_from_keycode(keycode);
        return true;
    }

    return false;
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

/**
 * @brief return true if modifiers and keycodes are set to 0, else false
 * @param keyboard_report
 */
bool is_keyboard_report_empty(struct usb_keyboard_report *report) {
    if (report->modifiers != KC_MOD_NONE) return false;

    for (uint8_t i = 0; i < KRO; i++) {
        if (report->keycodes[i] != KC_NONE) return false;
    }

    return true;
}

bool keyboard_report_cmp(struct usb_keyboard_report *x, struct usb_keyboard_report *y) {
    return (memcmp(x, y, sizeof(struct usb_keyboard_report)) == 0) ? true : false;
}

bool is_gamepad_report_empty(struct usb_gamepad_report *report) {
    return !(report->x | report->y | report->buttons);
}