#include "Keys.h"

#include "pico/stdlib.h"

//#include "tusb.h"
#include "../lib/tinyusb/usb_descriptors.h"

static int8_t current_layout;

void keys_init(void){
    gpio_pull_up(KEY1.pin);
    gpio_pull_up(KEY12.pin);
    current_layout = 0;
}

void send_keyboard_report(void){
    hid_keyboard_report_t key_state = {0};

    static bool has_sent_macro = false;
    static bool is_volume_control = false;
    
    /*if(!gpio_get(KEY1.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY1.layout_index],
            sizeof(hid_keyboard_report_t)
        );
        is_volume_control = true;
    }
    else if(!gpio_get(KEY12.pin)){
        memcpy(
            &key_state,
            &layouts[current_layout].keyboard_reports[KEY12.layout_index],
            sizeof(hid_keyboard_report_t)
        );
    }*/

    //if(!gp)

    if(is_volume_control && has_sent_macro){
        uint16_t vempty = 0;
        tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &vempty, 2);
        has_sent_macro = false;
        is_volume_control = false;
    }
    else if(is_volume_control){
        uint16_t vdown = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
        tud_hid_report(
            REPORT_ID_CONSUMER_CONTROL,
            &vdown,
            2
        );
        has_sent_macro = true;
        
    }

    else{

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


}