#!/bin/bash

BUILD_PATH="$PICO_KEYPAD_PATH/build"
UF2="$BUILD_PATH/pico-keypad.uf2"
ELF="$BUILD_PATH/pico-keypad.elf"
CHIP="rp2040"

# Loop through all arguments
for arg in "$@"; do
    if [ "$arg" == "-b" ] || [ "$arg" == "--build" ]; then
        cmake -S $PICO_KEYPAD_PATH -B $BUILD_PATH && cd $BUILD_PATH && make;
        # -DCMAKE_BUILD_TYPE=Debug
        # -G Ninja
    fi

    if [ "$arg" == "-f" ] || [ "$arg" == "--flash" ]; then
        if test -d $PICO_USB_DISK_PATH; then
            echo "COPYING UF2 to pi pico" && cp $UF2 $PICO_USB_DISK_PATH;
        else
            echo -e "\e[31mERROR: pico unmounted or unmounted as a mass storage device.\e[0m"
        fi
    fi

    if [ "$arg" == "-d" ] || [ "$arg" == "--debug" ]; then
        cd $BUILD_PATH && make -j4
        openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000" -c "program $ELF verify reset exit"
        openocd -f interface/cmsis-dap.cfg -f target/$CHIP.cfg -c "adapter speed 5000"
    fi
done
