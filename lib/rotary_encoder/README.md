## using callback

```C
void knob_cw_callback(uint32_t state) {
    // do something cool when the knob is moving to right!
}

void knob_ccw_callback(uint32_t state) {
    // do something cool when the knob is moving to left!
}

rotary_encoder_t knob = {
    .pin_DT = GPIO1,
    .pin_CLK = GPIO2,
    .state_CLK = LOW,
    .cw_callback = &knob_cw_callback,
    .ccw_callback = &knob_ccw_callback
};

rotary_encoder_init(&knob, true); // true to enable gpio pull up

while (true) {
    rotary_encoder_task();
}

```

## not using callback
```C
rotary_encoder_t knob = {
    .pin_DT = GPIO1,
    .pin_CLK = GPIO2,
};

rotary_encoder_init(&knob, true); // true to enable gpio pull up

while (true) {
    switch (rotary_encoder_read(&knob)) {
        case 1:
            // moving to right
            break;
        case -1:
            // moving to left
            break;
        case 0:
            // not moving
    }
}
```