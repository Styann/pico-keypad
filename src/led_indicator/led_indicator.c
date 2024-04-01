#include "led_indicator.h"

void init_led_indicator(struct led_indicator *led_indicator) {
    gpio_init(led_indicator->pin);
    gpio_set_dir(led_indicator->pin, GPIO_OUT);
}

void toggle_led_indicator(struct led_indicator *led_indicator) {
    /*uint8_t value = led_report & led_indicator->mask;

    if (led_indicator->state != value) {
        gpio_put(led_indicator->pin, !led_indicator->state);
    }*/
    gpio_put(led_indicator->pin, !led_indicator->state);
}