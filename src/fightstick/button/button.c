#include "button.h"

void button_init(uint8_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

/**
 * @brief returns button state, true = pressed, false = unpressed
 * @return bool
 */
bool button_read(uint8_t pin) {
    return !gpio_get(pin);
}
