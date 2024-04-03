#ifndef PICO_EXTRA_H
#define PICO_EXTRA_H

#define LOW 0
#define HIGH 1

enum GPIO {
    GPIO0 = 0,
    GPIO1,
    GPIO2,
    GPIO3,
    GPIO4,
    GPIO5,
    GPIO6,
    GPIO7,
    GPIO8,
    GPIO9,
    GPIO10,
    GPIO11,
    GPIO12,
    GPIO13,
    GPIO14,
    GPIO15,
    GPIO16,
    GPIO17,
    GPIO18,
    GPIO19,
    GPIO20,
    GPIO21,
    GPIO22,
    GPIO25 = 25,
    GPIO26 = 26,
    GPIO27 = 27,
    GPIO28 = 28,
};

static inline uint32_t millis(void) {
    return to_ms_since_boot(get_absolute_time());
}

#endif