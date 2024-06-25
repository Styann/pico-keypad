#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define JOYSTICK_UP_MASK    0b1000
#define JOYSTICK_RIGHT_MASK 0b0100
#define JOYSTICK_DOWN_MASK  0b0010
#define JOYSTICK_LEFT_MASK  0b0001

struct joystick {
    const uint8_t up_pin;
    const uint8_t right_pin;
    const uint8_t down_pin;
    const uint8_t left_pin;
};

void joystick_init(struct joystick *this);

uint8_t joystick_read(struct joystick *this);

static int8_t joystick_read_axis(uint8_t low_pin, uint8_t high_pin);

int8_t joystick_read_x_axis(struct joystick *this);

int8_t joystick_read_y_axis(struct joystick *this);

#endif
