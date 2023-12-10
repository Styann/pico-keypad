#include "Keys.h"

#include "pico/stdlib.h"

//#include "tusb.h"
#include "../lib/tinyusb/usb_descriptors.h"

#define HID_KEYBOARD_REPORT_SIZE sizeof(hid_keyboard_report_t)

void keys_init(void){

    for(uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++){
        gpio_init(rows_pins[r]);
        gpio_set_dir(rows_pins[r], GPIO_IN);
        gpio_pull_up(rows_pins[r]);
    }

    for(uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++){
        gpio_init(columns_pins[c]);
        gpio_set_dir(columns_pins[c], GPIO_IN);
        gpio_pull_up(columns_pins[c]);
    }

    return;
}

void send_keyboard_report(void){
    hid_keyboard_report_t key_state = {0};
    static bool has_sent_macro = false;

    scan_keypad_row(0, &key_state) ||
    scan_keypad_row(1, &key_state) ||
    scan_keypad_row(2, &key_state) ||
    scan_keypad_row(3, &key_state) || 
    scan_key(4, 0, &key_state);

    if(key_state.modifier != 0 || key_state.keycode[0] != 0){
        //to send modifier -> KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_LEFTCTRL
        has_sent_macro = true;
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, key_state.modifier, key_state.keycode);
    }
    else if(has_sent_macro){
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_sent_macro = false;
    }
    
    return;
    /*consumer control example
    uint16_t vempty = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vempty, 2);
    uint16_t vdown = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vdown, 2);*/
}

bool scan_keypad_row(uint8_t row, hid_keyboard_report_t *report){
    //I know the goto statement isn't recommended, but I wanted to use it.
    gpio_set_dir(rows_pins[row], GPIO_OUT);
    gpio_put(rows_pins[row], 0);
    busy_wait_us_32(1);

    if(!gpio_get(columns_pins[0])) {
        memcpy(report, &layout[row][0], HID_KEYBOARD_REPORT_SIZE);
        goto pressed;
    } 
    else if(!gpio_get(columns_pins[1])){
        memcpy(report, &layout[row][1], HID_KEYBOARD_REPORT_SIZE);
        goto pressed;
    }
    else if(!gpio_get(columns_pins[2])){
        memcpy(report, &layout[row][2], HID_KEYBOARD_REPORT_SIZE);
        goto pressed;
    }
    else if(!gpio_get(columns_pins[3])){
        memcpy(report, &layout[row][3], HID_KEYBOARD_REPORT_SIZE);
        goto pressed;
    }
    
    gpio_set_dir(rows_pins[row], GPIO_IN);  
    return false;

    pressed:
    gpio_set_dir(rows_pins[row], GPIO_IN);
    return true;
}

bool scan_key(uint8_t row, uint8_t column, hid_keyboard_report_t *report){
    gpio_set_dir(rows_pins[row], GPIO_OUT);
    gpio_put(rows_pins[row], 0);
    busy_wait_us_32(1);

    bool pin_value = !gpio_get(columns_pins[column]);

    if(pin_value){
        memcpy(report, &layout[row][column], HID_KEYBOARD_REPORT_SIZE);
    }

    gpio_set_dir(rows_pins[row], GPIO_IN);  
    return pin_value;
}