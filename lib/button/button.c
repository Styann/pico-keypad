#include "button.h"
#include "../pico_extra/pico_extra.h"

void button_init(button_t *this, bool internal_pull_up) {
    gpio_init(this->pin);
    gpio_set_dir(this->pin, GPIO_IN);
    if (internal_pull_up) gpio_pull_up(this->pin);
}

bool button_debounce(button_t *this) {
    if (millis() - this->timer > this->debounce_ms) {
        this->timer = millis();
        return true;
    }

    return false;
}

bool button_read(button_t *this) {
    const bool lastState = this->state;
    this->state = !gpio_get(this->pin);

    this->hasStateChanged = (this->state != lastState);

    return this->state;
}

bool button_is_pressed(button_t *this) {
    return button_read(this);
}

bool button_is_released(button_t *this) {
    return !button_read(this);
}

bool button_is_pressed_and_state_changed(button_t *this) {
    return (button_is_pressed(this) && this->hasStateChanged);
}

bool button_is_released_and_state_changed(button_t *this) {
    return (button_is_released(this) && this->hasStateChanged);
}

/**
 * @param timer
 * @param ms
 * @returns {bool}
 */
bool debounce(uint32_t *timer, uint32_t ms) {
    if (millis() - *timer > ms) {
        *timer = millis();
        return true;
    }

    return false;
}