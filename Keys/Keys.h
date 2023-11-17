#ifndef KEYS_H
#define KEYS_H

#define LAYOUT_LENGTH 4

typedef struct {
    uint8_t pin;
    uint8_t layout_index;
} key;

const key KEY_1  = {10, 0 };
const key KEY_2  = {11, 1 };
const key KEY_3  = {12, 2 };
const key KEY_4  = {13, 3 };
const key KEY_5  = {  , 4 }; 
const key KEY_6  = {  , 5 }; 
const key KEY_7  = {  , 6 }; 
const key KEY_8  = {  , 7 }; 
const key KEY_9  = {  , 8 }; 
const key KEY_10 = {  , 9 }; 
const key KEY_11 = {  , 10}; 
const key KEY_12 = {  , 11}; 
const key KEY_13 = {  , 12}; 
const key KEY_14 = {  , 13}; 
const key KEY_15 = {  , 14}; 
const key KEY_16 = {  , 15}; 
const key KEY_17 = {  , 16}; 

typedef struct {
    hid_keyboard_report_t keyboard_reports[LAYOUT_LENGTH];
} keyboard_layout;

static const keyboard_layout layouts[] = {
    {{
        {0, 0, {HID_KEY_KEYPAD_1, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_2, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_3, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_4, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_5, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_6, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_7, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_8, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_9, 0, 0, 0, 0, 0}},

        {0, 0, {HID_KEY_KEYPAD_ADD, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_MULTIPLY, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_SUBTRACT, 0, 0, 0, 0, 0}},
        {0, 0, {HID_KEY_KEYPAD_DIVIDE, 0, 0, 0, 0, 0}},

        {0, 0, {HID_KEY_KEYPAD_ENTER, 0, 0, 0, 0, 0}},

        

    }}
};

void init_keys(void);
void send_keyboard_report(void);

#endif