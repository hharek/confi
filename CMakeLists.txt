cmake_minimum_required(VERSION 3.9)
project(confi)

set(CMAKE_C_STANDARD 11)

set(confi_SOURCE_FILES
        confi.h
        confi_err.c
        confi_token.c
        confi_check.c
        confi.c
        main.c)

add_executable(confi ${confi_SOURCE_FILES})
