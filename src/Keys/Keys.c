#include "Keys.h"

#include "pico/stdlib.h"

//#include "tusb.h"
#include "../lib/tinyusb/usb_descriptors.h"

#define HID_KEYBOARD_REPORT_SIZE sizeof(hid_keyboard_report_t)

static int8_t current_layout;

void keys_init(void) {

    for(uint8_t r = 0; r < LAYOUT_ROW_LENGTH; r++) {
        gpio_init(rows_pins[r]);
        gpio_set_dir(rows_pins[r], GPIO_IN);
        gpio_pull_up(rows_pins[r]);
    }

    for(uint8_t c = 0; c < LAYOUT_COLUMN_LENGTH; c++) {
        gpio_init(columns_pins[c]);
        gpio_set_dir(columns_pins[c], GPIO_IN);
        gpio_pull_up(columns_pins[c]);
    }

    current_layout = 0;
}

void send_keyboard_report(void) {
    hid_keyboard_report_t key_state = {0};

    static bool has_sent_macro = false;
    uint8_t current_row = 0;

  
    // scan first row
    gpio_set_dir(rows_pins[current_row], GPIO_OUT);
    gpio_put(rows_pins[current_row], 0);
    busy_wait_us_32(1);

    if(!gpio_get(columns_pins[0])){
        memcpy(&key_state, &layout[current_row][0], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    else if(!gpio_get(columns_pins[1])){
        memcpy(&key_state, &layout[current_row][1], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    else if(!gpio_get(columns_pins[2])){
        memcpy(&key_state, &layout[current_row][2], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    else if(!gpio_get(columns_pins[3])){
        memcpy(&key_state, &layout[current_row][3], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    
    gpio_set_dir(rows_pins[current_row], GPIO_IN);
    //gpio_pull_up(rows_pins[current_row]);

    // scan second row
    current_row = 1;
    gpio_set_dir(rows_pins[current_row], GPIO_OUT);
    gpio_put(rows_pins[current_row], 0);
    busy_wait_us_32(1);


    if(!gpio_get(columns_pins[0])) {
        memcpy(&key_state, &layout[current_row][0], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    } 
    else if(!gpio_get(columns_pins[1])){
        memcpy(&key_state, &layout[current_row][1], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    else if(!gpio_get(columns_pins[2])){
        memcpy(&key_state, &layout[current_row][2], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    else if(!gpio_get(columns_pins[3])){
        memcpy(&key_state, &layout[current_row][3], HID_KEYBOARD_REPORT_SIZE);
        gpio_set_dir(rows_pins[current_row], GPIO_IN);
        goto send;
    }
    
    gpio_set_dir(rows_pins[current_row], GPIO_IN);
    // third row
    // fourth row
    // fifth row

    send:
    if(key_state.modifier != 0 || key_state.keycode[0] != 0){
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, key_state.modifier, key_state.keycode);
        has_sent_macro = true;
    }
    else if(has_sent_macro){
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_sent_macro = false;
    }
    
    //return;
    /*consumer control example
    uint16_t vempty = 0;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vempty, 2);
    uint16_t vdown = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
    tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vdown, 2);*/
}