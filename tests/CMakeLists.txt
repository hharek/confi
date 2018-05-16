cmake_minimum_required(VERSION 3.9)
project(confi_tests)

set(CMAKE_C_STANDARD 11)

set(confi_tests_SOURCE_FILES
        ../confi.h
        file.c
        main.c)

add_executable(tests ${confi_tests_SOURCE_FILES})

target_link_libraries(tests confi)

