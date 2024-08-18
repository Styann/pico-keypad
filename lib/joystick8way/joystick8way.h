#ifndef JOYSTICK8WAY_H
#define JOYSTICK8WAY_H

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "../button/button.h"

#define JOYSTICK8WAY_UP_MASK    0b1000
#define JOYSTICK8WAY_RIGHT_MASK 0b0100
#define JOYSTICK8WAY_DOWN_MASK  0b0010
#define JOYSTICK8WAY_LEFT_MASK  0b0001

struct joystick8way {
    // uint8_t debounce_ms;
    button_t up;
    button_t right;
    button_t down;
    button_t left;
};

void joystick8way_init(struct joystick8way *this);

uint8_t joystick8way_read(struct joystick8way *this);

static int8_t joystick8way_read_axis(button_t *low_button, button_t *high_button);

int8_t joystick8way_read_x_axis(struct joystick8way *this);

int8_t joystick8way_read_y_axis(struct joystick8way *this);

#endif
