#include "joystick8way.h"

void joystick8way_init(struct joystick8way *this) {
    // if (!this->debounce_ms) this->debounce_ms = 10;

    button_init(&this->up, true);
    button_init(&this->right, true);
    button_init(&this->down, true);
    button_init(&this->left, true);
}

uint8_t joystick8way_read(struct joystick8way *this) {
    uint8_t state = 0b0000;

    state |= button_read(&this->up) << 3;
    state |= button_read(&this->right) << 2;
    state |= button_read(&this->down) << 1;
    state |= button_read(&this->left);

    return state;
}

static int8_t joystick8way_read_axis(button_t *low_button, button_t *high_button) {
    if (button_read(low_button)) return -127;
    if (button_read(high_button)) return 127;
    return 0;
}

int8_t joystick8way_read_x_axis(struct joystick8way *this) {
    return joystick8way_read_axis(&this->left, &this->right);
}

int8_t joystick8way_read_y_axis(struct joystick8way *this) {
    return joystick8way_read_axis(&this->down, &this->up);
}
