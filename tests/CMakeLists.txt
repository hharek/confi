cmake_minimum_required(VERSION 3.9)
project(confi_tests)

set(CMAKE_C_STANDARD 11)

# token.h
add_executable(tests_token  ../confi.h
                            tests.h
                            tests.c
                            token.c)


# Запуск тестов
add_custom_target(tests
        COMMAND ./tests_token)
add_dependencies(tests tests_token)


