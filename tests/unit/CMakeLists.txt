cmake_minimum_required(VERSION 3.9)
project(confi_tests_unit)

set(CMAKE_C_STANDARD 11)

set(confi_tests_unit_token_SOURCE_FILES
        ../../token.h
        ../../token.c
        token.c)

add_executable(tests_unit_token ${confi_tests_unit_token_SOURCE_FILES})



