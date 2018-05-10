#pragma once

#include <stdbool.h>

/**
 * Константы
 */
enum
{
	CONFI_FILE_MAX_SIZE = 4096
};

/**
 * Типы параметров
 */
enum confi_type
{
	CONFI_TYPE_INT,
	CONFI_TYPE_DOUBLE,
	CONFI_TYPE_STRING,
	CONFI_TYPE_BOOLEAN
};

/**
 * Параметр
 */
struct confi_param
{
	const char * name;
	enum confi_type type;
	char * value;
	bool require;
	void * ptr;
};

/**
 * Спарсить файл
 */
int confi (const char * file, struct confi_param * params);

/**
 * Спарсить строку
 */
int confi_parse_string (const char * str, struct confi_param * params);

/**
 * Показать сообщение об ошибке
 */
char * confi_err ();
