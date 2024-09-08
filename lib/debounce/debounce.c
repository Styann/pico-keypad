#include "debounce.h"
#include "../pico_extra/pico_extra.h"

/**
 * @param timer
 * @param ms
 * @returns {bool}
 */
bool debounce(uint32_t *timer, const uint32_t ms) {
    if (millis() - *timer > ms) {
        *timer = millis();
        return true;
    }

    return false;
}
