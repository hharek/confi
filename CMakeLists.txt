cmake_minimum_required(VERSION 3.9)
project(confi)

set(CMAKE_C_STANDARD 11)

set(confi_SOURCE_FILES
        config.h
        config.c
        main.c)

add_executable(confi ${confi_SOURCE_FILES})
