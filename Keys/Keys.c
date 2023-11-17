#include "Keys.h"

#include "pico/stdlib.h"

#include "tusb.h"
#include "../lib/tinyusb/usb_descriptors.h"

static int8_t current_layout;

void init_keys(void){
    gpio_pull_up(KEY_1.pin);
    gpio_pull_up(KEY_2.pin);
    gpio_pull_up(KEY_3.pin);
    gpio_pull_up(KEY_4.pin);
    current_layout = 0;
}

void send_keyboard_report(void){
    hid_keyboard_report_t key_state = {0};

    static bool has_sent_macro = false;
    
    if(!gpio_get(KEY_1.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY_1.layout_index],
            sizeof(hid_keyboard_report_t)
        );
    }
    else if(!gpio_get(KEY_2.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY_2.layout_index],
            sizeof(hid_keyboard_report_t)
        );
    }
    else if(!gpio_get(KEY_3.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY_3.layout_index],
            sizeof(hid_keyboard_report_t)
        );
    }
    else if(!gpio_get(KEY_4.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY_4.layout_index],
            sizeof(hid_keyboard_report_t)
        );
    }

    if(key_state.modifier != 0 || key_state.keycode[0] != 0){
        tud_hid_keyboard_report(
            REPORT_ID_KEYBOARD,
            key_state.modifier,
            key_state.keycode
        );

        has_sent_macro = true;
    }
    else if(has_sent_macro){
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
        has_sent_macro = false;
    }
}