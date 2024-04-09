#include "hw040.h"

void hw040_init(struct hw040 *hw040) {
    // init CLK pin
    gpio_init(hw040->pin_CLK);
    gpio_set_dir(hw040->pin_CLK, GPIO_IN);

    // init DT pin
    gpio_init(hw040->pin_DT);
    gpio_set_dir(hw040->pin_DT, GPIO_IN);

    // init SW pin (button)
    gpio_init(hw040->pin_SW);
    gpio_set_dir(hw040->pin_SW, GPIO_IN);
    gpio_pull_up(hw040->pin_SW);
    gpio_set_irq_enabled(hw040->pin_SW, GPIO_IRQ_EDGE_RISE, true);
}

/**
 * @param {struct hw040} *hw040
 */
void hw040_task(struct hw040 *hw040) {
    hw040->state_CLK = gpio_get(hw040->pin_CLK);

    if (hw040->state_CLK != hw040->last_state_CLK) {
        if (gpio_get(hw040->pin_DT) != hw040->state_CLK) {
            hw040->cw_callback();
        }
        else {
            hw040->ccw_callback();
        }
    }

    hw040->last_state_CLK = hw040->state_CLK;
}