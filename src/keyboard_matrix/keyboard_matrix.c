#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "keyboard_matrix.h"
#include "layout.h"
#include "../pico_extra.h"
#include "../usb/usb.h"

#define KRO 6 // Key RollOver, number of keys that can be pressed at once

/**
 * @brief Set all rows pins as OUPUT and HIGH then all columns pins as GPIO INPUT PULL UP 
 */
void keyboard_matrix_init(void) {
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
 * @param report
 */
void keyboard_matrix_scan(struct usb_keyboard_report *report) {
    uint8_t pressed_keys_count = 0;

    for (uint8_t r = 0; pressed_keys_count < KRO && r < LAYOUT_ROW_LENGTH; r++) {
        gpio_put(rows_pins[r], LOW);
        busy_wait_us_32(1);

        for (uint8_t c = 0; pressed_keys_count < KRO && c < LAYOUT_COLUMN_LENGTH; c++) {
            if (is_key_pressed(columns_pins[c]) && layout[r][c] != KC_NONE) {
                set_keyboard_report(report, layout[r][c]);
                pressed_keys_count++;
            }
        }

        gpio_put(rows_pins[r], HIGH);
    }
}

/**
 * @brief Add a keycode to the keyboard report, and possibly his modifier btw
 * @param report
 * @param keycode
 */
static void set_keyboard_report(struct usb_keyboard_report *report, uint8_t keycode) {
    if (keycode > KC_CTRL_LEFT && keycode < KC_GUI_RIGHT) {
        report->modifier |= get_modifier_from_keycode(keycode);
    }

    if (report->keycode[0] == 0) report->keycode[0] = keycode;
    else if (report->keycode[1] == 0) report->keycode[1] = keycode;
    else if (report->keycode[2] == 0) report->keycode[2] = keycode;
    else if (report->keycode[3] == 0) report->keycode[3] = keycode;
    else if (report->keycode[4] == 0) report->keycode[4] = keycode;
    else if (report->keycode[5] == 0) report->keycode[5] = keycode;
}

/**
 * @brief Compare two keyboard report, return true if they are equal
 * @param dest
 * @param src
 */
bool has_keyboard_report_changed(struct usb_keyboard_report *dest, struct usb_keyboard_report *src) {
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
bool is_keyboard_report_empty(struct usb_keyboard_report *report) {
    if (report->modifier != 0x00) return false;
    if (report->keycode[0] != 0x00) return false;
    if (report->keycode[1] != 0x00) return false;
    if (report->keycode[2] != 0x00) return false;
    if (report->keycode[3] != 0x00) return false;
    if (report->keycode[4] != 0x00) return false;
    if (report->keycode[5] != 0x00) return false;
    return true;
}

/**
 * @brief Take a keycode and return his modifier if he has one 
 * @param keycode must be between KC_CTRL_LEFT (0xE0) and KC_GUI_RIGHT (0xE7)
 */
static uint8_t get_modifier_from_keycode(uint8_t keycode) {
    return (0x01 << (keycode & 0b00001111));
}

/**
 * @brief Send keyboard report to host
 */
void usb_send_keyboard_report(struct usb_keyboard_report *report){
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_keyboard_report));
}

/**
 * @brief send report with all fields at 0
 */
void release_keyboard(void) {
    struct usb_keyboard_report release = { 0x01, 0, 0, { 0, 0, 0, 0, 0, 0 } };
    usb_send_keyboard_report(&release);
}

/**
 * @brief Send consumer control report to host
 */
void usb_send_consumer_control_report(struct usb_consumer_control_report *report) {
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_consumer_control_report));
}

/**
 * @brief send consumer control, then release
 * @param consumer_control 
 */
void usb_send_consumer_control(uint16_t consumer_control) {
    struct usb_consumer_control_report report = { 0x02, consumer_control };

    // send
    usb_send_consumer_control_report(&report);

    // release
    report.consumer_control = 0x0000;
    usb_send_consumer_control_report(&report);
}

void usb_send_gamepad_report(struct usb_gamepad_report *report) {
    struct usb_endpoint *endpoint = usb_get_endpoint_configuration(EP_IN_HID);
    usb_start_transfer(endpoint, (uint8_t*)report, sizeof(struct usb_gamepad_report));
}
