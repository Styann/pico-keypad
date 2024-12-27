#!/bin/bash

export PICO_KEYPAD_PATH="$HOME/Workspace/pico-keypad"
BUILD_PATH="$PICO_KEYPAD_PATH/build-debug"
BIN="$BUILD_PATH/pico-keypad.elf"

gdb-multiarch $BIN -ex "target remote localhost:3333" -ex "monitor reset init"

# monitor reset -

