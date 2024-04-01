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
}

void rotary_encoder_init(struct rotary_encoder encoder);

void rotary_encoder_read(struct rotary_encoder encoder);

#endif