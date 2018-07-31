cmake_minimum_required(VERSION 3.7)
project(confi_example)

set(CMAKE_C_STANDARD 11)

add_executable(example ../confi.h example.c)
add_executable(example_allow ../confi.h example_allow.c)
add_executable(example_error ../confi.h example_error.c)
add_executable(example_parse_string ../confi.h example_parse_string.c)

target_link_libraries(example confi)
target_link_libraries(example_allow confi)
target_link_libraries(example_error confi)
target_link_libraries(example_parse_string confi)

install(FILES example.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
install(FILES example_allow.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})