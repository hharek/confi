cmake_minimum_required(VERSION 3.9)
project(confi_tests_integration)

set(CMAKE_C_STANDARD 11)

set(confi_tests_integration_SOURCE_FILES
        ../../confi.h
        file.c
        param.c
        token.c
        value.c
        main.c)

add_executable(tests_integration ${confi_tests_integration_SOURCE_FILES})

target_link_libraries(tests_integration confi)

