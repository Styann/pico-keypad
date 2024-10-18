#include "color.h"

struct rgb rgb(uint8_t r, uint8_t g, uint8_t b) {
    const struct rgb color = { r, g, b };
    return color;
}

struct grb grb(uint8_t g, uint8_t r, uint8_t b) {
    const struct grb color = { g, r, b };
    return color;
}
