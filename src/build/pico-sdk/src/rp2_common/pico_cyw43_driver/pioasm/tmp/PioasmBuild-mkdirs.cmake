# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Pico/pico-sdk/tools/pioasm"
  "/Pico/pico-keypad/src/build/pioasm"
  "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm"
  "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
  "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Pico/pico-keypad/src/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/PioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
