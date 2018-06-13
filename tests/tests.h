#pragma once

#include <stdbool.h>

/**
 * Структура для тестов
 */
struct test
{
	char * name;
	bool (*func) ();
};

/**
 * Выполнение тестов
 */
static int tests (struct test * tests);