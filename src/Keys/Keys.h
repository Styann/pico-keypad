#ifndef KEYS_H
#define KEYS_H

#include "tusb.h"

#define LAYOUT_LENGTH 17
#define LAYOUT_COLUMN_LENGTH 4
#define LAYOUT_ROW_LENGTH 5

typedef struct {
    uint8_t pin;
    //uint8_t layout_index;
    hid_keyboard_report_t report;
} key;

/*static const key KEY1  = {13, {0, 0, {HID_KEY_NUM_LOCK       , 0, 0, 0, 0, 0}}};
static const key KEY2  = {0,  {0, 0, {HID_KEY_KEYPAD_DIVIDE  , 0, 0, 0, 0, 0}}};
static const key KEY3  = {0,  {0, 0, {HID_KEY_KEYPAD_MULTIPLY, 0, 0, 0, 0, 0}}};
static const key KEY4  = {0,  {0, 0, {HID_KEY_KEYPAD_SUBTRACT, 0, 0, 0, 0, 0}}};
static const key KEY5  = {0,  {0, 0, {HID_KEY_KEYPAD_7       , 0, 0, 0, 0, 0}}}; 
static const key KEY6  = {0,  {0, 0, {HID_KEY_KEYPAD_8       , 0, 0, 0, 0, 0}}}; 
static const key KEY7  = {0,  {0, 0, {HID_KEY_KEYPAD_9       , 0, 0, 0, 0, 0}}}; 
static const key KEY8  = {0,  {0, 0, {HID_KEY_KEYPAD_ADD     , 0, 0, 0, 0, 0}}}; 
static const key KEY9  = {0,  {0, 0, {HID_KEY_KEYPAD_4       , 0, 0, 0, 0, 0}}}; 
static const key KEY10 = {0,  {0, 0, {HID_KEY_KEYPAD_5       , 0, 0, 0, 0, 0}}}; 
static const key KEY11 = {0,  {0, 0, {HID_KEY_KEYPAD_6       , 0, 0, 0, 0, 0}}}; 
static const key KEY12 = {14, {0, 0, {HID_KEY_KEYPAD_1       , 0, 0, 0, 0, 0}}}; 
static const key KEY13 = {0,  {0, 0, {HID_KEY_KEYPAD_2       , 0, 0, 0, 0, 0}}}; 
static const key KEY14 = {0,  {0, 0, {HID_KEY_KEYPAD_3       , 0, 0, 0, 0, 0}}}; 
static const key KEY15 = {0,  {0, 0, {HID_KEY_KEYPAD_ENTER   , 0, 0, 0, 0, 0}}}; 
static const key KEY16 = {0,  {0, 0, {HID_KEY_KEYPAD_0       , 0, 0, 0, 0, 0}}}; 
static const key KEY17 = {0,  {0, 0, {HID_KEY_KEYPAD_DECIMAL , 0, 0, 0, 0, 0}}};*/

static const uint8_t columns_pins[LAYOUT_COLUMN_LENGTH] = {2, 3, 4, 5};
static const uint8_t rows_pins[LAYOUT_ROW_LENGTH] = {11, 12, 13, 14, 15};

static const hid_keyboard_report_t layout[LAYOUT_ROW_LENGTH][LAYOUT_COLUMN_LENGTH] = {
    {
        //{0, 0, {HID_KEY_NUM_LOCK       , 0, 0, 0, 0, 0}},
        {HID_KEY_SHIFT_RIGHT, 0, {0     , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_DIVIDE  , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_MULTIPLY, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_SUBTRACT, 0, 0, 0, 0, 0}}
    },
    {
        //{0, 0, {HID_KEY_KEYPAD_7       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_A     , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_8       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_9       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_ADD     , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_KEYPAD_4       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_5       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_6       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_ENTER   , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_KEYPAD_1       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_2       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_3       , 0, 0, 0, 0, 0}}, 
        {0, 0, {HID_KEY_KEYPAD_0       , 0, 0, 0, 0, 0}}
    },
    {
        {0, 0, {HID_KEY_KEYPAD_DECIMAL , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_NONE           , 0, 0, 0, 0, 0}}
    }
};


void keys_init(void);
void send_keyboard_report(void);
        
/*typedef struct {
    hid_keyboard_report_t keyboard_reports[LAYOUT_LENGTH];
} keyboard_layout;

static const keyboard_layout layouts[] = {
    {{
        {0, 0, {HID_KEY_NUM_LOCK       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_DIVIDE  , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_MULTIPLY, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_SUBTRACT, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_7       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_8       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_9       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_ADD     , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_4       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_5       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_6       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_1       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_2       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_3       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_ENTER   , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_0       , 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_DECIMAL , 0, 0, 0, 0, 0}}
    }}
};*/

#endif