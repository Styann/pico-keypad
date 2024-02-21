//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "pico_extra.h"

// usb
#include "usb/usb.h"

// keys
#include "keys/keys.h"

// leds
#include "leds/leds.h"

#define LED_PIN GPIO22
// #define TOGGLE_PIN GPIO22
// #define LED_PIN2 GPIO25

void hid_task(void);
bool keyboard_task_callback(struct repeating_timer *timer);

int main(void) {
    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);
    gpio_put(GPIO25, HIGH);

    stdio_init_all();

    /*gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, HIGH);*/

    //gpio_init(LED_PIN2);
    //gpio_set_dir(LED_PIN2, GPIO_OUT);

    struct leds rgb_strip = {
        { GPIO22, 0, 0 },
        { GPIO22, 0, 0 },
        { GPIO22, 0, 0 },
        32
    };

    leds_init(&rgb_strip);
    leds_put_color(&rgb_strip, 0x606060);

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

    usb_device_init();
    keys_init();

    while (!is_configured()) {
        tight_loop_contents();
    }

    // every 10ms the keyboard matrix will be scanned
    /*struct repeating_timer keyboard_timer;
    add_repeating_timer_ms(
        10, //10ms
        keyboard_task_callback,
        NULL,
        &keyboard_timer
    );*/
    
    while (true) {

        hid_task();
        //tight_loop_contents();
    }
    
    return 0;
}

bool keyboard_task_callback(struct repeating_timer *timer) {

    struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 } };
    //struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };

    scan_keyboard(&keyboard_report);
    usb_send_hid_keyboard_report(&keyboard_report);

    return true;
}

void hid_task(void) {
    static uint32_t timer = 0;
    const uint32_t interval_ms = 10;

    if (to_ms_since_boot(get_absolute_time()) - timer > interval_ms) {
        timer = to_ms_since_boot(get_absolute_time());

        struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 } };
        //struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };
        scan_keyboard(&keyboard_report);
        usb_send_hid_keyboard_report(&keyboard_report);
    }

    return;
}
