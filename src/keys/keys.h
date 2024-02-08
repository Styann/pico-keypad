#ifndef KEYS_H
#define KEYS_H

#include "pico/stdlib.h"
#include "../usb/usb_hid.h"

#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5

typedef struct {
    uint8_t keycode;
    uint8_t modifier;
    uint16_t consumer_control;
} keyboard_key_t;

void keys_init(void);

bool is_key_pressed(uint8_t column_pin);

bool is_keyboard_report_empty(struct usb_hid_keyboard_report *keyboard_report);

void isr_scan_keyboard(void);

void set_keyboard_report(struct usb_hid_keyboard_report *keyboard_report, keyboard_key_t *key);

uint8_t get_modifier_from_keycode(uint8_t keycode);

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {2, 3, 4, 5};
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = {11, 12, 13, 14, 15};

static keyboard_key_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    {
        {KC_NUM_LOCK       , 0, 0},
        {KC_KEYPAD_DIVIDE  , 0, 0},
        {KC_KEYPAD_MULTIPLY, 0, 0},
        {KC_KEYPAD_SUBTRACT, 0, 0}
    },
    {
        {KV_KEYPAD_7       , 0, 0},
        {KC_KEYPAD_8       , 0, 0},
        {KC_KEYPAD_9       , 0, 0}, 
        {KC_KEYPAD_ADD     , 0, 0}
    },
    {
        {KC_KEYPAD_4       , 0, 0}, 
        {KC_KEYPAD_5       , 0, 0}, 
        {KC_KEYPAD_6       , 0, 0}, 
        {KC_KEYPAD_ENTER   , 0, 0}
    },
    {
        {KC_KEYPAD_1       , 0, 0}, 
        {KC_KEYPAD_2       , 0, 0}, 
        {KC_KEYPAD_3       , 0, 0}, 
        {KC_KEYPAD_0       , 0, 0}
    },
    {
        {KC_KEYPAD_DECIMAL , 0, 0},
        {KC_NONE           , 0, 0},
        {KC_NONE           , 0, 0},
        {KC_NONE           , 0, 0}
    }
};

#endif