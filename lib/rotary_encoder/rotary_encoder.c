/**
 * @author Styann
 */

#include "rotary_encoder.h"

#include "../pico_extra/pico_extra.h"
#include "hardware/gpio.h"

void rotary_encoder_init(rotary_encoder_t *this, bool internal_pull_up) {
    this->state = 0;

    gpio_init(this->pin_CLK);
    gpio_init(this->pin_DT);

    gpio_set_dir(this->pin_CLK, GPIO_IN);
    gpio_set_dir(this->pin_DT, GPIO_IN);

    if (internal_pull_up) {
        gpio_pull_up(this->pin_CLK);
        gpio_pull_up(this->pin_DT);
    }
}

/**
 * @brief read and return direction
 * @returns {int8_t} clockwise: 1, unmoving: 0, counter_clockwise: 1
 */
int8_t rotary_encoder_read(rotary_encoder_t *this) {
    const bool last_state_CLK = this->state_CLK;
    this->state_CLK = gpio_get(this->pin_CLK);

    if (this->state_CLK != last_state_CLK) {
        if (gpio_get(this->pin_DT) != this->state) {
            return 1;
        }
        else {
            return -1;
        }
    }

    return 0;
}

/**
 * @brief read direction, then execute cw or ccw callback
 */
void rotary_encoder_task(rotary_encoder_t *this) {
    const bool last_state_CLK = this->state_CLK;
    this->state_CLK = gpio_get(this->pin_CLK);

    if (this->state_CLK != last_state_CLK) {
        if (gpio_get(this->pin_DT) != this->state_CLK) {
            this->cw_callback(++this->state);
        }
        else {
            this->ccw_callback(--this->state);
        }
    }
}
