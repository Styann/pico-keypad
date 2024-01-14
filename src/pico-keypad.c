//#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "usb/dev_lowlevel.h"
//#include "Keys/Keys.h"
#define LED_PIN 16

bool keyboard_timer_callback(struct repeating_timer *t);

int main(void) {   
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, true);

    stdio_init_all();
    usb_device_init();
    //keys_init();

    while(!is_configured()){
        tight_loop_contents();

    }

    // Get ready to rx from host
    usb_start_transfer(usb_get_endpoint_configuration(EP1_OUT_ADDR), NULL, 64);

    /*struct repeating_timer keyboard_timer;
    add_repeating_timer_ms(
        10, //10ms
        keyboard_timer_callback,
        NULL,
        &keyboard_timer
    );*/

    struct usb_hid_keyboard_report keyboard_report = {0, 0, {0Xe0, 0, 0, 0, 0, 0}};

    while(true){
        tight_loop_contents();
        usb_send_hid_keyboard_report(&keyboard_report);
        sleep_ms(10);
    }
    
    return 0;
}

/*bool keyboard_timer_callback(struct repeating_timer *t){
    if(tud_suspended()){
        tud_remote_wakeup();
    }
    else if(tud_hid_ready()){
        irq_scan_keyboard();
    }



    return true;
}*/