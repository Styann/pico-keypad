#include "joystick.h"
#include "../button/button.h"

void joystick_init(struct joystick *this) {
    button_init(this->up_pin);
    button_init(this->right_pin);
    button_init(this->down_pin);
    button_init(this->left_pin);
}

uint8_t joystick_read(struct joystick *this) {
    uint8_t state = 0b0000;

    state |= button_read(this->up_pin) << 3;
    state |= button_read(this->right_pin) << 2;
    state |= button_read(this->down_pin) << 1;
    state |= button_read(this->left_pin);

    return state;
}

static int8_t joystick_read_axis(uint8_t low_pin, uint8_t high_pin) {
    if (button_read(low_pin)) return -127;
    if (button_read(high_pin)) return 127;
    return 0;
}

int8_t joystick_read_x_axis(struct joystick *this) {
    return joystick_read_axis(this->left_pin, this->right_pin);
}

int8_t joystick_read_y_axis(struct joystick *this) {
    return joystick_read_axis(this->down_pin, this->up_pin);
}
