cmake_minimum_required(VERSION 3.7)
project(confi_example)

set(CMAKE_C_STANDARD 11)

add_executable(example ../confi.h main.c)

target_link_libraries(example confi)

install(FILES main.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})