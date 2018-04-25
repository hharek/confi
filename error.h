#pragma once

#include <stdarg.h>

static enum
{
	ERROR_MAX_SIZE = 2048			/* Максимальный размер текста ошибки */
};

/**
 * Последнее сообщение об ошибке
 */
static char * err[ERROR_MAX_SIZE];

/**
 * Назначить сообщение об ошибке
 */
static void error (char * format, ... );



