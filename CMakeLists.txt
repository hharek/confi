cmake_minimum_required(VERSION 3.9)
project(confi)

set(CMAKE_C_STANDARD 11)

set(confi_SOURCE_FILES
        confi.h
        confi.c
        error.h
        error.c
        check.h
        check.c
        token.h
        token.c
        value.h
        value.c)

add_library(confi SHARED ${confi_SOURCE_FILES})

add_subdirectory(example)
add_subdirectory(tests)