#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "pico_extra.h"

#include "usb/usb.h"
#include "keys/keys.h"
#include "leds/leds.h"

#define LED_PIN GPIO22

void hid_task(void);
void keyboard_task(void);
void leds_task(void);
//bool keyboard_task_callback(struct repeating_timer *timer);

int main(void) {
    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);
    //gpio_put(GPIO25, HIGH);

    const struct leds rgb_strip = {
        { GPIO22, 0, 0 },
        { GPIO26, 0, 0 },
        { GPIO27, 0, 0 },
        32
    };

    stdio_init_all();
    usb_device_init();
    keys_init();
    leds_init(&rgb_strip);
    leds_put_color(&rgb_strip, 0x606060);


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
        // tight_loop_contents();
    }
    
    return 0;
}

void hid_task(void) {
    static uint32_t timer = 0;
    const uint32_t interval_ms = 10;

    if (to_ms_since_boot(get_absolute_time()) - timer > interval_ms) {
        timer = to_ms_since_boot(get_absolute_time());
        
        keyboard_task();
        leds_task();
    }

    return;
}

void keyboard_task(void) {
    struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };
    //struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };
    scan_keyboard(&keyboard_report);
    usb_send_keyboard_report(&keyboard_report);
    
    return;
}

void leds_task(void) {
    return;
}



/*bool keyboard_task_callback(struct repeating_timer *timer) {

    struct usb_hid_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };
    //struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };

    scan_keyboard(&keyboard_report);
    usb_send_keyboard_report(&keyboard_report);

    return true;
}*/
