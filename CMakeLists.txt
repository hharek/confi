cmake_minimum_required(VERSION 3.7)
project(confi)

set(CMAKE_C_STANDARD 11)

# Library confi
set(confi_SOURCE_FILES
        confi.h
        confi.c)

add_library(confi SHARED ${confi_SOURCE_FILES})

# Examples and tests
add_subdirectory(examples)
add_subdirectory(tests)

# For command: make install
include(GNUInstallDirs)

set_target_properties(${PROJECT_NAME} PROPERTIES
        VERSION 1.1
        PUBLIC_HEADER confi.h)

install(TARGETS confi
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})