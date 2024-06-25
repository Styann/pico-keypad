#ifndef BUTTON_H
#define BUTTON_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

void button_init(uint8_t pin);

bool button_read(uint8_t pin);

bool debounce(uint32_t *timer, uint32_t ms);

#endif
