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
 * Коды ошибок программы
 */
enum confi_err_code
{
	CONFI_SUCCESS,								/* Ура */
	CONFI_ERR_FILE_NOT_OPEN,					/* Невозможно открыть файл */
	CONFI_ERR_FILE_NOT_FILE,					/* Указан не файл */
	CONFI_ERR_FILE_MAX_SIZE,					/* Большой размер файла */
	CONFI_ERR_FILE_EMPTY,						/* Для файла указана пустая строка */
	CONFI_ERR_FILE_NO_TEXT,						/* Файл не является текстовым */
	CONFI_ERR_PARAM_REPEAT,						/* Параметр повторяется */
	CONFI_ERR_PARAM_REQUIRE,					/* Отсутствует обязательный параметр */
	CONFI_ERR_PARAM_UNKNOWN,					/* Неизвестный параметр */
	CONFI_ERR_PARAM_NAME_FIRST_BAD_SYMBOL,		/* Недопустимый начальный символ в названии параметра */
	CONFI_ERR_PARAM_NAME_BAD_SYMBOL,			/* Недопустимые символы в названии параметра */
	CONFI_ERR_TOKEN_ORDER,						/* Неверный порядок токенов */
	CONFI_ERR_VALUE_NOT_INT,					/* Параметр не является числом */
	CONFI_ERR_VALUE_NOT_DOUBLE,					/* Параметр не является числом с плавающей запятой */
	CONFI_ERR_VALUE_BIG_STRING,					/* Параметр содержит очень длинную строку */
	CONFI_ERR_VALUE_NOT_BOOLEAN					/* Параметр не является булёвым значением */
};

/**
 * Структура с текстом ошибки
 */
struct confi_err
{
	enum confi_err_code code;
	char param[1024];
	char message[2048];
};

/**
 * Показать сообщение об ошибке
 */
struct confi_err * confi_err ();
