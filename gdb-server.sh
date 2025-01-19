#!/bin/bash

BUILD_PATH="$ANESIDORA_PATH/build-debug"
MAKEFILE_PATH="$BUILD_PATH/Makefile"
BIN="$BUILD_PATH/anesidora.elf"
CHIP="rp2040"

if ! test -f $MAKEFILE_PATH; then
    echo "executing 'cmake'"
    cmake -DCMAKE_BUILD_TYPE=Debug -S $ -B $BUILD_PATH
fi

cd $BUILD_PATH && make -j4

openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000" -c "program $BIN verify reset exit"

openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000"
