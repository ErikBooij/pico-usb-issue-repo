cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(macro_keypad C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(FAMILY rp2040)

pico_sdk_init()

add_compile_options(
  -Wall
  -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
  -Wno-unused-function # we have some for the docs that aren't called
  -Wno-maybe-uninitialized
)

add_executable(macro_keypad)

target_sources(macro_keypad PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/main.c
  ${CMAKE_CURRENT_LIST_DIR}/tusb_config.h
  ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
  ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.h
)

include(${PICO_SDK_PATH}/lib/tinyusb/hw/bsp/family_support.cmake)

target_sources(macro_keypad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/usb_descriptors.c
        )

target_include_directories(macro_keypad PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

family_configure_device_example(macro_keypad)