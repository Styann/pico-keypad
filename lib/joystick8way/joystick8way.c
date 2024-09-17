#include "joystick8way.h"

void joystick8way_init(struct joystick8way *self) {
    // if (!self->debounce_ms) self->debounce_ms = 10;

    button_init(&self->up, true);
    button_init(&self->right, true);
    button_init(&self->down, true);
    button_init(&self->left, true);
}

uint8_t joystick8way_read(struct joystick8way *self) {
    uint8_t state = 0b0000;

    state |= button_read(&self->up) << 3;
    state |= button_read(&self->right) << 2;
    state |= button_read(&self->down) << 1;
    state |= button_read(&self->left);

    return state;
}

static int8_t joystick8way_read_axis(button_t *low_button, button_t *high_button) {
    if (button_read(low_button)) return -127;
    if (button_read(high_button)) return 127;
    return 0;
}

int8_t joystick8way_read_x_axis(struct joystick8way *self) {
    return joystick8way_read_axis(&self->left, &self->right);
}

int8_t joystick8way_read_y_axis(struct joystick8way *self) {
    return joystick8way_read_axis(&self->down, &self->up);
}
