#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "keyboard_matrix.h"
#include "../pico_extra.h"
#include "../usb/usb.h"

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
                add_keycode(report, keycode);
                pressed_keys_count++;
            }
        }

        gpio_put(this->rows_pins[r], HIGH);
    }
}

/**
 * @brief add a keycode to the keyboard report, and possibly his modifier
 * @param report
 * @param keycode
 */
static void add_keycode(struct usb_keyboard_report *report, uint8_t keycode) {
    if (KC_CTRL_LEFT <= keycode && keycode <= KC_GUI_RIGHT) {
        report->modifiers |= get_modifier_from_keycode(keycode);
    }

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
    if (report->modifiers == KC_MOD_NONE) return false;

    for (uint8_t i = 0; i < KRO; i++) {
        if (report->keycodes[i] != KC_NONE) return false;
    }

    return true;
}

/**
 * @brief take a keycode and return his modifier if he has one
 * @param keycode must be between KC_CTRL_LEFT (0xE0) and KC_GUI_RIGHT (0xE7)
 */
static uint8_t get_modifier_from_keycode(uint8_t keycode) {
    return (0x01 << (keycode & 0b00001111));
}

/**
 * @brief send keyboard report to host
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
 * @brief send consumer control report to host
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

bool is_gamepad_report_empty(struct usb_gamepad_report *report) {
    return (report->x | report->y | report->buttons);
}
