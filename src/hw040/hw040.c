#include "hw040.h"

void hw040_init(struct hw040 *this) {
    // init CLK pin
    gpio_init(this->pin_CLK);
    gpio_set_dir(this->pin_CLK, GPIO_IN);

    // init DT pin
    gpio_init(this->pin_DT);
    gpio_set_dir(this->pin_DT, GPIO_IN);

    // init SW pin (button)
    gpio_init(this->pin_SW);
    gpio_set_dir(this->pin_SW, GPIO_IN);
    gpio_pull_up(this->pin_SW);
    gpio_set_irq_enabled(this->pin_SW, GPIO_IRQ_EDGE_RISE, true);
}

/**
 * @param {struct hw040} *hw040
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