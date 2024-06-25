#include "button.h"
#include "../pico_extra.h"

/**
 * @param pin
 */
void button_init(uint8_t pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
}

/**
 * @brief returns button state, true = pressed, false = unpressed
 * @param pin
 * @returns {bool}
 */
bool button_read(uint8_t pin) {
    return !gpio_get(pin);
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