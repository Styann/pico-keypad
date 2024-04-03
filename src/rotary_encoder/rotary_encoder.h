#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "pico/stdlib.h"
#include "../pico_extra.h"

struct rotary_encoder {
    uint8_t pin_SW; // switch button pin
    uint8_t pin_DT; // direction pin
    uint8_t pin_CLK; // rotation pin (CLK = click)
    
    uint16_t state;
    bool state_CLK;
    bool last_state_CLK;
    bool state_SW;

    void (*cw_callback)(void);
    void (*ccw_callback)(void);
};

void rotary_encoder_init(struct rotary_encoder *encoder, gpio_irq_callback_t rotate_cb, gpio_irq_callback_t sw_cb);

int8_t rotary_encoder_read(struct rotary_encoder *encoder);
//, void (*ccw_dt_cb)(void), void (*cw_dt_cb)(void)

#endif