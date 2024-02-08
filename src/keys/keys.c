#include "keys.h"
#include "../usb/dev_lowlevel.h"

#include "hardware/gpio.h"
#include "hardware/timer.h"

#define HIGH 1
#define LOW 0
#define HID_KEYBOARD_REPORT_SIZE sizeof(struct usb_hid_keyboard_report)

/**
 * @brief Set all rows pins as GPIO INPUT then all columns pins as GPIO INPUT and pull up 
 */
void keys_init(void){

    for (uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++){
        gpio_init(rows_pins[r]);
        gpio_set_dir(rows_pins[r], GPIO_IN);
    }

    for (uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++){
        gpio_init(columns_pins[c]);
        gpio_set_dir(columns_pins[c], GPIO_IN);
        gpio_pull_up(columns_pins[c]);
    }

    return;
}

/**
 * @brief Return whether the a key is pressed or not
 * @param column_pin
 */
bool is_key_pressed(uint8_t column_pin) {
    return !gpio_get(column_pin);
};

/**
 * @brief Return true if all fields of the keyboard report are set to 0, else false
 * @param keyboard_report 
 */
bool is_keyboard_report_empty(struct usb_hid_keyboard_report *keyboard_report) {
    if (keyboard_report->modifier != 0x00) return false;
    if (keyboard_report->keycode[0] != 0x00) return false;
    if (keyboard_report->keycode[1] != 0x00) return false;
    if (keyboard_report->keycode[2] != 0x00) return false;
    if (keyboard_report->keycode[3] != 0x00) return false;
    if (keyboard_report->keycode[4] != 0x00) return false;
    if (keyboard_report->keycode[5] != 0x00) return false;

    return true;
}

/**
 * @brief Loop through the matrix and add a the pressed key to the keyboard report then send it
 */
void isr_scan_keyboard(void){
    struct usb_hid_keyboard_report keyboard_report = {0, 0, {0, 0, 0, 0, 0, 0}};
    static bool has_sent_report = false;

    for (uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++) {
        // set selected row as a output
        gpio_set_dir(rows_pins[r], GPIO_OUT);
        // set power to LOW
        gpio_put(rows_pins[r], LOW);
        busy_wait_us_32(1);

        for (uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++) {
            if (layout[r][c].keycode != KC_NONE) {
                if (is_key_pressed(columns_pins[c])) {
                    set_keyboard_report(&keyboard_report, &layout[r][c]);
                } 
            }
        }

        //reset power to HIGH
        gpio_put(rows_pins[r], HIGH);
        // reset row as an input
        gpio_set_dir(rows_pins[r], GPIO_IN); 
    }
    
    if (!is_keyboard_report_empty(&keyboard_report)) {
        //to send modifier -> KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL
        usb_send_hid_keyboard_report(&keyboard_report);

        struct usb_hid_keyboard_report empty_keyboard_report = {0, 0, {0, 0, 0, 0, 0, 0}};
        usb_send_hid_keyboard_report(&empty_keyboard_report);
        has_sent_report = true;
    }
    else {
        has_sent_report = false;
    }

    //struct usb_hid_consumer_control_report consumer = {KC_MEDIA_VOLUME_DECREMENT};
    //usb_send_hid_consumer_control_report(&consumer);

    return;

    /*consumer control example
    uint16_t vempty = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vempty, 2);
    uint16_t vdown = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vdown, 2);*/
}

/**
 * @brief Add a keycode or a modifier key to the keyboard report
 * @param keyboard_report
 * @param key
 */
void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, keyboard_key_t *key){

    if (key->keycode > KC_CTRL_LEFT && key->keycode < KC_GUI_RIGHT) {
        keyboard_report->modifier |= get_modifier_from_keycode(key->keycode);
    }
    /*else if (key->consumer_control != 0){}*/
    
    
    if (keyboard_report->keycode[0] == 0) keyboard_report->keycode[0] = key->keycode;
    else if (keyboard_report->keycode[1] == 0) keyboard_report->keycode[1] = key->keycode;
    else if (keyboard_report->keycode[2] == 0) keyboard_report->keycode[2] = key->keycode;
    else if (keyboard_report->keycode[3] == 0) keyboard_report->keycode[3] = key->keycode;
    else if (keyboard_report->keycode[4] == 0) keyboard_report->keycode[4] = key->keycode;
    else if (keyboard_report->keycode[5] == 0) keyboard_report->keycode[5] = key->keycode;

    return;
}

/**
 * @brief Take a keycode and return his modifier if he has one 
 * @param keycode
*/
uint8_t get_modifier_from_keycode(uint8_t keycode){
    return (0x01 << (keycode & 0b00001111));
}
