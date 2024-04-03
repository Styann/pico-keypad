#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"
#include "pico_extra.h"

#include "usb/usb.h"
#include "keys/keys.h"
//#include "rgb_led_strip/rgb_led_strip.h"
#include "led_indicator/led_indicator.h"
#include "rotary_encoder/rotary_encoder.h"

#define LED_PIN GPIO22

void hid_task(void);
void keyboard_task(void);
void leds_task(void);

void gpio_volume_knob_rotate_callback(uint gpio, uint32_t events);
void gpio_volume_knob_sw_callback(uint gpio, uint32_t events);

//bool keyboard_task_callback(struct repeating_timer *timer);

struct rotary_encoder volume_knob = {
    .pin_SW = GPIO28,
    .pin_DT = GPIO27,
    .pin_CLK = GPIO26,
    .state = LOW,
    .state_CLK = LOW,
    .last_state_CLK = HIGH,
    .state_SW = HIGH,
};

void gpio_volume_knob_rotate_callback(uint gpio, uint32_t events) {
    volume_knob.state_CLK = gpio_get(volume_knob.pin_CLK);

    if (gpio_get(volume_knob.pin_DT) != volume_knob.state_CLK) {
        volume_knob.state++;
        gpio_put(25, 1);
    }
    else {
        volume_knob.state--;
        gpio_put(25, 0);
    }

    volume_knob.last_state_CLK = volume_knob.state_CLK;
}

void gpio_volume_knob_sw_callback(uint gpio, uint32_t events) {
    struct usb_keyboard_report report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, KC_MEDIA_PLAY_PAUSE };
    usb_send_keyboard_report(&report);
    
    static bool state = 0;

    state = !state,
    gpio_put(25, state);
}

int main(void) {

    gpio_init(GPIO25);
    gpio_set_dir(GPIO25, GPIO_OUT);

    rotary_encoder_init(
        &volume_knob,
        &gpio_volume_knob_rotate_callback,
        &gpio_volume_knob_sw_callback
    );

    stdio_init_all();
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
        


        // hid_task();
        tight_loop_contents();
    }
    
    return 0;
}


void hid_task(void) {
    static uint32_t timer = 0;
    const uint32_t interval_ms = 10;

    if (millis() - timer > interval_ms) {
        timer = to_ms_since_boot(get_absolute_time());
        
        //keyboard_task();
        // leds_task();
    }

    return;
}

void keyboard_task(void) {
    struct usb_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };

    scan_keyboard(&keyboard_report);

    //usb_send_keyboard_report(&keyboard_report);
}

void leds_task(void) {
    return;
}




/*bool keyboard_task_callback(struct repeating_timer *timer) {

    struct usb_keyboard_report keyboard_report = { 0, 0, { 0, 0, 0, 0, 0, 0 }, 0 };
    //struct usb_hid_consumer_control_report consumer_report = { CONSUMER_CONTROL_REPORT_ID, 0x0000 };

    scan_keyboard(&keyboard_report);
    usb_send_keyboard_report(&keyboard_report);

    return true;
}*/


void thread_core1() {

}

void thread_core2() {
    
}
