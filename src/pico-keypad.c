//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

//usb
#include "usb/dev_lowlevel.h"

//keys
#include "keys/keys.h"

#define LED_PIN 16
#define TOGGLE_PIN 17

void hid_task(void);
bool keyboard_timer_callback(struct repeating_timer *timer);

int main(void) { 
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, true);

    /*bool state;
    gpio_init(TOGGLE_PIN);
    gpio_set_dir(TOGGLE_PIN, GPIO_IN);
    gpio_pull_up(TOGGLE_PIN);
    bool previous_state = gpio_get(TOGGLE_PIN);

    while (true) {
        state = gpio_get(TOGGLE_PIN);

        if (state != previous_state) {

            if (!state) {
                gpio_put(LED_PIN, false);
            } 
            else {
                gpio_put(LED_PIN, true);
            }
        }

        sleep_ms(300);
    }*/
    stdio_init_all();
    usb_device_init();
    keys_init();

    while (!is_configured()) {
        tight_loop_contents();
    }

    // every 10ms the keyboard matrix will be scanned
    struct repeating_timer keyboard_timer;
    add_repeating_timer_ms(
        10, //10ms
        keyboard_timer_callback,
        NULL,
        &keyboard_timer
    );

    while (true) {
        //hid_task();
        tight_loop_contents();
    }
    
    return 0;
}


bool keyboard_timer_callback(struct repeating_timer *timer){

    static bool has_sent_keyboard_report = false;
    static bool has_sent_consumer_report = false;

    struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 } };
    struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };

    scan_keyboard(&keyboard_report);

    if (!is_keyboard_report_empty(&keyboard_report)) {

        // if certain combination of key then send consumer control
        if (false) {
            usb_send_hid_consumer_control_report(&consumer_report);
            has_sent_consumer_report = true;
        }
        else {
            usb_send_hid_keyboard_report(&keyboard_report);
            has_sent_keyboard_report = true;
        }

    }
    else if (has_sent_keyboard_report){
        usb_send_hid_keyboard_report(&keyboard_report);
        //has_sent_keyboard_report = false;
    }
    else if (has_sent_consumer_report) {
        usb_send_hid_consumer_control_report(&consumer_report);
    }

    return true;
}

void hid_task(void) {
    const uint32_t interval_ms = 10;
    static uint32_t start_ms = 0;
    static bool has_sent_report = false;

    if (to_ms_since_boot(get_absolute_time()) - start_ms < interval_ms) {
        return;
        // continue if this function is called before 10ms
    }

    struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0x0D, 0, 0, 0, 0, 0 } };
    start_ms += interval_ms;

    
    //scan_keyboard(&keyboard_report);

    if (!is_keyboard_report_empty(&keyboard_report)) {
        usb_send_hid_keyboard_report(&keyboard_report);
        has_sent_report = true;
    }
    else {
        usb_send_hid_keyboard_report(&keyboard_report);
        has_sent_report = false;
    }

    return;
}
