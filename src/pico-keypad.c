#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#include "tusb.h"
//#include "bsp/board.h"
#include "lib/tinyusb/usb_descriptors.h"

#include "Keys/Keys.h"


bool keyboard_timer_callback(struct repeating_timer *t);

int main(void) {   
    
    //stdio_init_all();
    tusb_init();
    keys_init();

    struct repeating_timer keyboard_timer;

    add_repeating_timer_ms(
        10, //10ms
        keyboard_timer_callback,
        NULL,
        &keyboard_timer
    );

    while(true){
        tud_task();

        //gpio_put(LED_PIN, 1);
    }
    
    return 0;
}

bool keyboard_timer_callback(struct repeating_timer *t){
    if(tud_suspended()){
        tud_remote_wakeup();
    }
    else if(tud_hid_ready()){
        send_keyboard_report();
    }

    return true;
}




// TinyUSB HID callbacks

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen){
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize){

}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len) {}


// TinyUSB Device callbacks

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts) {}

void tud_mount_cb() {}

void tud_unmount_cb() {}

void tud_suspend_cb(bool remote_wakeup_en) {}

void tud_resume_cb() {}