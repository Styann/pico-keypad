#ifndef COLOR_H
#define COLOR_H

#include "pico/types.h"

#define RGB_BIT_SIZE 24
#define GRB_BIT_SIZE 24

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct grb {
    uint8_t g;
    uint8_t r;
    uint8_t b;
};

struct rgb rgb(uint8_t r, uint8_t g, uint8_t b);

struct grb grb(uint8_t g, uint8_t r, uint8_t b);

// #define GRB_GREEN   0x00FF0000u
// #define GRB_RED     0x0000FF00u
// #define GRB_BLUE    0x000000FFu
// #define GRB_CYAN    0x00FF00FFu
// #define GRB_YELLOW  0x00FFFF00u
// #define GRB_MAGENTA 0x0000FFFFu
// #define GRB_ORANGE  0x0017FF00u
// #define GRB_WHITE   0x00FFFFFFu
// #define GRB_OFF     0x00000000u
// #define GRB_G_MASK 0x00FF0000u
// #define GRB_R_MASK 0x0000FF00u
// #define GRB_B_MASK 0x000000FFu

#endif
