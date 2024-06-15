/**
 * @author Styann
 */

#include "hw040.h"
#include "../pico_extra.h"
#include "hardware/gpio.h"

void hw040_init(struct hw040 *this, bool enable_switch) {
    // init CLK pin
    gpio_init(this->pin_CLK);
    gpio_set_dir(this->pin_CLK, GPIO_IN);

    // init DT pin
    gpio_init(this->pin_DT);
    gpio_set_dir(this->pin_DT, GPIO_IN);

    // TODO: review this
    // init SW pin (button)
    if (enable_switch) {
        gpio_init(this->pin_SW);
        gpio_set_dir(this->pin_SW, GPIO_IN);
        gpio_pull_up(this->pin_SW);
        gpio_set_irq_enabled(this->pin_SW, GPIO_IRQ_EDGE_FALL, true);
    }
}

/**
 * @brief read direction, then execute cw or ccw callback
 */
void hw040_task(struct hw040 *this) {
    this->state_CLK = gpio_get(this->pin_CLK);

    if (this->state_CLK != this->last_state_CLK) {
        if (gpio_get(this->pin_DT) != this->state_CLK) {
            this->cw_callback();
        }
        else {
            this->ccw_callback();
        }
    }

    this->last_state_CLK = this->state_CLK;
}
