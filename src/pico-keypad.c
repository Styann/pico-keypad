//#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/timer.h"

#include "usb/dev_lowlevel.h"
//#include "Keys/Keys.h"


bool keyboard_timer_callback(struct repeating_timer *t);

int main(void) {   
    
    stdio_init_all();
    usb_device_init();
    //keys_init();

    while(!is_configured()){
        tight_loop_contents();

    }
    //struct repeating_timer keyboard_timer;

    /*add_repeating_timer_ms(
        10, //10ms
        keyboard_timer_callback,
        NULL,
        &keyboard_timer
    );*/

    // Get ready to rx from host
    usb_start_transfer(usb_get_endpoint_configuration(EP1_OUT_ADDR), NULL, 64);


    while(true){
        tight_loop_contents();
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