#!/bin/bash

export PICO_KEYPAD_PATH="$HOME/Workspace/pico-keypad"
BUILD_PATH="$PICO_KEYPAD_PATH/build-debug"
MAKEFILE_PATH="$BUILD_PATH/Makefile"
BIN="$BUILD_PATH/pico-keypad.elf"
CHIP="rp2040"

if ! test -f $MAKEFILE_PATH; then
    echo "executing 'cmake'"
    cmake -DCMAKE_BUILD_TYPE=Debug -S $PICO_KEYPAD_PATH -B $BUILD_PATH
fi

cd $BUILD_PATH && make -j4

openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000" -c "program $BIN verify reset exit"

openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000"
