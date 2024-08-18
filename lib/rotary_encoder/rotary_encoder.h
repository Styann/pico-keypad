/**
 * @author Styann
 */

#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "pico/stdlib.h"

typedef struct rotary_encoder {
    uint8_t pin_DT;
    uint8_t pin_CLK;
    bool state_CLK;
    uint32_t state;
    void (*cw_callback)(uint32_t state);
    void (*ccw_callback)(uint32_t state);
} rotary_encoder_t;

void rotary_encoder_init(rotary_encoder_t *this, bool internal_pull_up);

int8_t rotary_encoder_read(rotary_encoder_t *this);

void rotary_encoder_task(rotary_encoder_t *this);

#endif
