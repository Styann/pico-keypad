#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "keys.h"
#include "layout.h"
#include "../pico_extra.h"
#include "../usb/usb.h"

#define KRO 6 // Key RollOver, number of keys that can be pressed at once

/**
 * @brief Set all rows pins as OUPUT and HIGH then all columns pins as GPIO INPUT PULL UP 
 */
void keys_init(void) {

    for (uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++){
        gpio_init(rows_pins[r]);
        gpio_set_dir(rows_pins[r], GPIO_OUT);
        gpio_put(rows_pins[r], HIGH);
    }

    for (uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++){
        gpio_init(columns_pins[c]);
        gpio_set_dir(columns_pins[c], GPIO_IN);
        gpio_pull_up(columns_pins[c]);
    }

    return;
}

/**
 * @brief Return whether a key is pressed or not
 * @param column_pin
 */
static bool is_key_pressed(uint8_t column_pin) {
    return !gpio_get(column_pin);
};

/**
 * @brief Set pin as OUTPUT and set power to LOW
 * @param pin
 */
static inline void set_pin_output_write_low(uint8_t pin) {
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, LOW);
}

/**
 * @brief Set pin as OUTPUT and set power to HIGH
 * @param pin
 */
static inline void set_pin_output_write_high(uint8_t pin) {
    gpio_set_dir(pin, GPIO_OUT);
    gpio_put(pin, HIGH);
}

/**
 * @brief Loop through the matrix and add the keys that are pressed to a keyboard report
 * @param keyboard_report
 */
void scan_keyboard(struct usb_hid_keyboard_report *keyboard_report) {
    uint8_t pressed_keys_count = 0;

    for (uint8_t r = 0; pressed_keys_count < KRO && r < LAYOUT_ROW_LENGTH; r++) {
        
        set_pin_output_write_low(rows_pins[r]);
        busy_wait_us_32(1);

        for (uint8_t c = 0; pressed_keys_count < KRO && c < LAYOUT_COLUMN_LENGTH; c++) {

            if (is_key_pressed(columns_pins[c]) && layout[r][c] != KC_NONE) {
                set_keyboard_report(keyboard_report, layout[r][c]);

                if (layout[r][c] == KC_1) {
                    keyboard_report->consumer_control = KC_MEDIA_VOLUME_INCREMENT;
                }
                pressed_keys_count++;
            }
        }

        set_pin_output_write_high(rows_pins[r]);
    }

    return;
}

/**
 * @brief Add a keycode to the keyboard report, and possibly his modifier btw
 * @param keyboard_report
 * @param keycode
 */
static void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, uint8_t keycode) {

    if (keycode > KC_CTRL_LEFT && keycode < KC_GUI_RIGHT) {
        keyboard_report->modifier |= get_modifier_from_keycode(keycode);
    }
    
    if (keyboard_report->keycode[0] == 0) keyboard_report->keycode[0] = keycode;
    else if (keyboard_report->keycode[1] == 0) keyboard_report->keycode[1] = keycode;
    else if (keyboard_report->keycode[2] == 0) keyboard_report->keycode[2] = keycode;
    else if (keyboard_report->keycode[3] == 0) keyboard_report->keycode[3] = keycode;
    else if (keyboard_report->keycode[4] == 0) keyboard_report->keycode[4] = keycode;
    else if (keyboard_report->keycode[5] == 0) keyboard_report->keycode[5] = keycode;
    return;
}

/**
 * @brief Compare two keyboard report, return true if they are equal
 * @param dest
 * @param src
 */
bool assert_keyboard_reports(struct usb_hid_keyboard_report *dest, struct usb_hid_keyboard_report *src) {
    if (dest->modifier != src->modifier) return false;
    if (dest->keycode[0] != src->keycode[0]) return false;
    if (dest->keycode[1] != src->keycode[1]) return false;
    if (dest->keycode[2] != src->keycode[2]) return false;
    if (dest->keycode[3] != src->keycode[3]) return false;
    if (dest->keycode[4] != src->keycode[4]) return false;
    if (dest->keycode[5] != src->keycode[5]) return false;
    return true;
}

/**
 * @brief Return true if all fields of the keyboard report are set to 0, else false
 * @param keyboard_report 
 */
bool is_keyboard_report_empty(struct usb_hid_keyboard_report *keyboard_report) {
    struct usb_hid_keyboard_report empty = { 0, 0, { 0, 0, 0, 0, 0, 0 } };
    return assert_keyboard_reports(&empty, keyboard_report);
}

/**
 * @brief Take a keycode and return his modifier if he has one 
 * @param keycode must be between KC_CTRL_LEFT (0xE0) and KC_GUI_RIGHT (0xE7) 
 */
static uint8_t get_modifier_from_keycode(uint8_t keycode) {
    return (0x01 << (keycode & 0b00001111));
}

void release(void) {
    struct usb_hid_keyboard_report empty_keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };
    usb_send_keyboard_report(&empty_keyboard_report);
}
