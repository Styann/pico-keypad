#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"
#include "../pico_extra/pico_extra.h"

typedef struct {
    uint8_t pin;
    bool state;
} led_t;

void led_init(led_t *const led);

void led_put(led_t *const led, const bool state);

void led_toggle(led_t *const led);

void led_on(led_t *const led);

void led_off(led_t *const led);

#endif
