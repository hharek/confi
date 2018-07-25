cmake_minimum_required(VERSION 3.7)
project(confi)

set(CMAKE_C_STANDARD 11)

set(confi_SOURCE_FILES
        confi.h
        confi.c
        err.h
        err.c
        check.h
        check.c
        token.h
        token.c
        value.h
        value.c)

add_library(confi SHARED ${confi_SOURCE_FILES})

add_subdirectory(example)
add_subdirectory(tests)

install(TARGETS confi DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/lib)
install(FILES confi.h DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include)