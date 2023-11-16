#include "Keys.h"

#include "pico/stdlib.h"

#include "tusb.h"
#include "../lib/tinyusb/usb_descriptors.h"

void init_keys(){
    gpio_pull_up(KEY);
}

hid_keyboard_report_t key1 = {0, 0, {HID_KEY_1, 0, 0, 0, 0, 0}};

void send_keyboard_report(){
    hid_keyboard_report_t key_state = {0};

    static bool has_sent_macro = false;
    
    if(!gpio_get(KEY)){
        memcpy(&key_state, &key1, sizeof(hid_keyboard_report_t));
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