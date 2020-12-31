cmake_minimum_required(VERSION 3.10)

project(XCAN    VERSION 1.0
                DESCRIPTION "XCAN Routing Stack"
                LANGUAGES C)

add_executable(XCAN_EXE
                modules/xcan_dev_socketcan.c
                stack/xcan_device.c
                stack/xcan_frame.c
                stack/xcan_stack.c
                examples/linux/main.c)

target_include_directories(XCAN_EXE PUBLIC
                           "stack"
                           "stack/include"
                           "modules"
                           "examples/linux")