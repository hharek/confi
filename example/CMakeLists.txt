cmake_minimum_required(VERSION 3.9)
project(confi_example)

set(CMAKE_C_STANDARD 11)

add_executable(example ../confi.h main.c)

target_link_libraries(example confi)