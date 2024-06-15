/**
 * @author Styann
 */

#ifndef HW040_H
#define HW040_H

#include "pico/stdlib.h"

struct hw040 {
    uint8_t pin_SW; // switch button pin
    uint8_t pin_DT; // direction pin
    uint8_t pin_CLK; // rotation pin (CLK = click)
    bool state_CLK;
    bool last_state_CLK;
    void (*cw_callback)(void);
    void (*ccw_callback)(void);
};

void hw040_init(struct hw040 *this, bool enable_switch);

void hw040_task(struct hw040 *this);

#endif
