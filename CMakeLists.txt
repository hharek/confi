cmake_minimum_required(VERSION 3.9)
project(confi)

set(CMAKE_C_STANDARD 11)

set(confi_SOURCE_FILES
        confi.h
        confi.c
        error.h
        error.c
        token.h
        token.c
        value.h
        value.c
        main.c)

add_executable(confi ${confi_SOURCE_FILES})
