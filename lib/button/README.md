```C
button_t btn = { .pin = 1, .debounce_ms = 5 };

button_init(&btn, true); // true to enable gpio pull up


while (true) {
    if (button_debounce(&btn)) {
        if (button_is_pressed(&btn)) {
            // do something cool!
        }
        else {
            // do another thing cool!
        }
    }
}
```