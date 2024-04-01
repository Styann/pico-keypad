#include "rotary_encoder.h"
 
void rotary_encoder_init(struct rotary_encoder *encoder, gpio_irq_callback_t irq_cb) {
    // init DT pin
    gpio_init(encoder->pin_DT);
    gpio_set_dir(encoder->pin_DT, GPIO_IN);
    gpio_set_irq_enabled_with_callback(encoder->pin_DT, , true, );

    // init CLK pin
    gpio_init(encoder->pin_CLK);
    gpio_set_dir(encoder->pin_CLK, GPIO_IN);
    gpio_set_irq_enabled_with_callback();
}

/**
 * @param {struct rotary_encoder} encoder
 * @param {function} ccw_dt_cb - left rotation callback
 * @param {function} cw_dt_cb - right rotation callback
*/
void rotary_encoder_read(struct rotary_encoder *encoder, void (*ccw_dt_cb)(void), void (*cw_dt_cb)(void)) {
    encoder->state_CLK = gpio_get(encoder->pin_CLK);

    if ((!encoder->last_state_CLK) && (encoder->state_CLK)) {
        
        if (gpio_get(encoder->pin_DT) != encoder->state_CLK) {
            encoder->state--;
            ccw_dt_cb();
        }
        else {
            encoder->state++;
            cw_dt_cb();
        }
    }

    encoder->last_state_CLK = encoder->state_CLK;
}