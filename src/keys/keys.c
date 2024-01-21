#include "keys.h"
#include "../usb/dev_lowlevel.h"

#include "hardware/gpio.h"
#include "hardware/timer.h"

#define HIGH 1
#define LOW 0
#define HID_KEYBOARD_REPORT_SIZE sizeof(struct usb_hid_keyboard_report)

void keys_init(void){

    for(uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++){
        gpio_init(rows_pins[r]);
        gpio_set_dir(rows_pins[r], GPIO_IN);
    }

    for(uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++){
        gpio_init(columns_pins[c]);
        gpio_set_dir(columns_pins[c], GPIO_IN);
        gpio_pull_up(columns_pins[c]);
    }

    return;
}


void isr_scan_keyboard(void){
    struct usb_hid_keyboard_report key_state = {0, 0, {0, 0, 0, 0, 0, 0}};
    static bool has_sent_report = false;

    for(uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++){
        // set selected row as a output
        gpio_set_dir(rows_pins[r], GPIO_OUT);
        // set power to LOW
        gpio_put(rows_pins[r], LOW);
        busy_wait_us_32(1);

        for(uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++){
            if(layout[r][c].keycode != HID_KEY_NONE){
                if(!gpio_get(columns_pins[c])) {
                    set_keyboard_report(&key_state, &layout[r][c]);
                } 
            }
        }

        //reset power to HIGH
        gpio_put(rows_pins[r], HIGH);
        // reset row as an input
        gpio_set_dir(rows_pins[r], GPIO_IN); 
    }
    
    if(key_state.modifier != 0 || key_state.keycode[0] != 0){
        //to send modifier -> KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL
        usb_send_hid_keyboard_report(&key_state);
        has_sent_report = true;
    } else{
        usb_send_hid_keyboard_report(&key_state);
        has_sent_report = false;
    }

    return;

    /*scan_keypad_row(&key_state, 0);
    scan_keypad_row(&key_state, 1);
    scan_keypad_row(&key_state, 2);
    scan_keypad_row(&key_state, 3);
    scan_key(&key_state, 4, 0);*/

    /*consumer control example
    uint16_t vempty = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vempty, 2);
    uint16_t vdown = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vdown, 2);*/
}

/**
 * @brief Add a keycode or a modifier key to the keyboard report
 */
void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, keyboard_key_t *key){

    if(key->modifier != 0){
        keyboard_report->modifier |= key->modifier;
    }
    /*else if(key->consumer_control != 0){}*/
    else{
        if(keyboard_report->keycode[0] == 0) keyboard_report->keycode[0] = key->keycode;
        else if(keyboard_report->keycode[1] == 0) keyboard_report->keycode[1] = key->keycode;
        else if(keyboard_report->keycode[2] == 0) keyboard_report->keycode[2] = key->keycode;
        else if(keyboard_report->keycode[3] == 0) keyboard_report->keycode[3] = key->keycode;
        else if(keyboard_report->keycode[4] == 0) keyboard_report->keycode[4] = key->keycode;
        else if(keyboard_report->keycode[5] == 0) keyboard_report->keycode[5] = key->keycode;
    }

    return;
}

// deprecated / unused
void scan_keypad_row(struct usb_hid_keyboard_report *keyboard_report, uint8_t row){
    gpio_set_dir(rows_pins[row], GPIO_OUT);
    gpio_put(rows_pins[row], 0);
    busy_wait_us_32(1);

    if(!gpio_get(columns_pins[0])) {
        set_keyboard_report(keyboard_report, &layout[row][0]);
    } 
    if(!gpio_get(columns_pins[1])){
        set_keyboard_report(keyboard_report, &layout[row][1]);
    }
    if(!gpio_get(columns_pins[2])){
        set_keyboard_report(keyboard_report, &layout[row][2]);
    }
    if(!gpio_get(columns_pins[3])){
        set_keyboard_report(keyboard_report, &layout[row][3]);
    }

    gpio_set_dir(rows_pins[row], GPIO_IN); 
    return;
}

// deprecated / unused
void scan_key(struct usb_hid_keyboard_report *keyboard_report, uint8_t row, uint8_t column){
    gpio_set_dir(rows_pins[row], GPIO_OUT);
    gpio_put(rows_pins[row], 0);
    busy_wait_us_32(1);

    if(!gpio_get(columns_pins[column])){
        set_keyboard_report(keyboard_report, &layout[row][column]);
    }

    gpio_set_dir(rows_pins[row], GPIO_IN);  
    return;
}