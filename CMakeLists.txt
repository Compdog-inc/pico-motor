cmake_minimum_required(VERSION 3.30)

# Pull in SDK (must be before project)
include("$ENV{PICO_SDK_PATH}\\external\\pico_sdk_import.cmake")

project(pico-motor C CXX ASM)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
        message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

if(NOT PICO_MOTOR_USED_STATE_MACHINES)
        set(PICO_MOTOR_USED_STATE_MACHINES 0,1,2,3,5,6) #4 does not work for some reason
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

configure_file( config.h.in ${CMAKE_BINARY_DIR}/generated/pico-motor/config.h )
include_directories( ${CMAKE_BINARY_DIR}/generated/pico-motor/ ) 

add_library(pico-motor STATIC
        src/motor.cpp
        # pio
        src/pio/driverpwm.c
        )
        
pico_generate_pio_header(pico-motor ${CMAKE_CURRENT_LIST_DIR}/src/pio/driverpwm.pio)

target_include_directories(pico-motor PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/..
        )
target_link_libraries(pico-motor
        pico_stdlib
        hardware_pio
        )