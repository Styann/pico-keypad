#include "led.h"

void led_init(led_t *const led) {
    gpio_init(led->pin);
    gpio_set_dir(led->pin, GPIO_OUT);
    gpio_put(led->pin, led->state);
}

void led_put(led_t *const led, const bool state) {
    gpio_put(led->pin, state);
    led->state = state;
}

void led_toggle(led_t *const led) {
    const bool new_state = !(led->state);
    led_put(led, new_state);
}

void led_on(led_t *const led) {
    led_put(led, HIGH);
}

void led_off(led_t *const led) {
    led_put(led, LOW);
}
