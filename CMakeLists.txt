cmake_minimum_required(VERSION 3.10)

project(XCAN    VERSION 1.0
                DESCRIPTION "XCAN Routing Stack"
                LANGUAGES C)

add_executable(XCAN_EXE
                modules/can_dev_softdevice.c
                stack/can_device.c
                stack/can_frame.c
                examples/linux/main.c)

target_include_directories(XCAN_EXE PUBLIC
                           "stack"
                           "stack/include"
                           "modules"
                           "examples/linux")