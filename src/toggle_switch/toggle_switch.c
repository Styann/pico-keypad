    bool state;
    gpio_init(TOGGLE_PIN);
    gpio_set_dir(TOGGLE_PIN, GPIO_IN);
    gpio_pull_up(TOGGLE_PIN);
    bool previous_state = gpio_get(TOGGLE_PIN);

    while (true) {
        state = gpio_get(TOGGLE_PIN);

        if (state != previous_state) {

            if (!state) {
                gpio_put(LED_PIN, false);
            } 
            else {
                gpio_put(LED_PIN, true);
            }
        }

        sleep_ms(300);
    }
