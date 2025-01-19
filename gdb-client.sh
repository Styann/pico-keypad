#!/bin/bash

BUILD_PATH="$ANESIDORA_PATH/build-debug"
BIN="$BUILD_PATH/anesidora.elf"

gdb-multiarch $BIN -ex "target remote localhost:3333" -ex "monitor reset init"

# monitor reset -
