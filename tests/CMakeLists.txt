cmake_minimum_required(VERSION 3.9)
project(confi_tests)

set(CMAKE_C_STANDARD 11)

# token.h
add_executable(tests_token  ../confi.h
                            tests.h
                            tests.c
                            token.c)

# check.h
add_executable(tests_check  ../confi.h
                            tests.h
                            tests.c
                            check.c)

# Запуск тестов
add_custom_target(tests
        COMMAND ./tests_token
        COMMAND ./tests_check)

# Зависемости
add_dependencies(tests tests_token tests_check)


