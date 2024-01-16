/*#ifndef LAYOUT_H
#define LAYOUT_H

#include "pico/stdlib"
#include "../usb/usb_hid.h"

#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5


typedef struct {
    uint8_t keycode;
    uint8_t modifier;
    uint8_t consumer_control;
} keyboard_key_t;

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {2, 3, 4, 5};
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = {11, 12, 13, 14, 15};

static const uint16_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    {HID_KEY_1, HID_KEY_2, HID_KEY_3, HID_KEY_4, HID_KEY_5, HID_KEY_6, HID_KEY_7, HID_KEY_8, HID_KEY_9, HID_KEY_0},
    
};

#endif*/