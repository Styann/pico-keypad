#include "rotary_encoder.h"

void rotary_encoder_init(struct rotary_encoder *encoder, gpio_irq_callback_t rotate_cb, gpio_irq_callback_t sw_cb) {
    // init CLK pin
    gpio_init(encoder->pin_CLK);
    gpio_set_dir(encoder->pin_CLK, GPIO_IN);
    gpio_disable_pulls(encoder->pin_CLK);

    // init DT pin
    gpio_init(encoder->pin_DT);
    gpio_set_dir(encoder->pin_DT, GPIO_IN);
    gpio_disable_pulls(encoder->pin_DT);

    // init irq callback on rotation change
    /*gpio_set_irq_enabled_with_callback(
        encoder->pin_CLK,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
        true,
        rotate_cb
    );*/

    // init encoder button
    gpio_init(encoder->pin_SW);
    gpio_set_dir(encoder->pin_SW, GPIO_IN);
    gpio_pull_up(encoder->pin_SW);

    gpio_set_irq_enabled_with_callback(
        encoder->pin_SW,
        GPIO_IRQ_EDGE_RISE,
        true,
        sw_cb
    );
}

/**
 * @brief read rotary encoder without interrupt, so I don't use it actually
 * @param {struct rotary_encoder} *encoder
 * @param {function} ccw_dt_cb - left rotation callback
 * @param {function} cw_dt_cb - right rotation callback
*/
int8_t rotary_encoder_read(struct rotary_encoder *encoder) {
    encoder->state_CLK = gpio_get(encoder->pin_CLK);

    // if (encoder->state_CLK && !encoder->last_state_CLK) {

    if (encoder->state_CLK != encoder->last_state_CLK) {
        
        if (gpio_get(encoder->pin_DT) != encoder->state_CLK) {
            encoder->state++;
        }
        else {
            encoder->state--;
        }
    }

    encoder->last_state_CLK = encoder->state_CLK;
}