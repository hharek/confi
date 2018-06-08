#include <stdio.h>
#include <stdlib.h>

#include "../../confi.h"

/* Параметры */
static struct confi_param confi_params_min[] =
{
	{.name = "param", .type = CONFI_TYPE_INT},
	NULL
};

static bool test_file_not_found ();
static bool test_file_not_file ();
static bool test_file_empty ();
static bool test_file_big_size ();
static bool test_file_no_text ();
static bool test_file_random_string ();
static bool test_file_content_empty ();

/**
 * Указан несуществующий файл
 */
bool test_file_not_found ()
{
	if (confi ("not_found", NULL) != CONFI_ERR_FILE_NOT_OPEN)
	{
		return false;
	}

	return true;
}

/**
 * Указан каталог вместо файла
 */
bool test_file_not_file ()
{
	if (confi (".", NULL) != CONFI_ERR_FILE_NOT_FILE)
	{
		return false;
	}

	return true;
}

/**
 * Не указан файл
 */
bool test_file_empty ()
{
	if (confi ("", NULL) != CONFI_ERR_FILE_EMPTY)
	{
		return false;
	}

	return true;
}

/**
 * Файл содержит большой размер
 */
bool test_file_big_size ()
{
	/* Генерируем большой файл */
	char file[255];
	tmpnam (file);

	FILE * fp = fopen (file, "w");

	int big_size = CONFI_FILE_MAX_SIZE + 1;
	for (int i = 0; i < big_size; i++)
	{
		fputc ((int)random (), fp);
	}

	fclose (fp);

	/* Передаём функции */
	if (confi (file, NULL) != CONFI_ERR_FILE_MAX_SIZE)
	{
		return false;
	}

	/* Удаляем старый файл */
	remove (file);

	return true;
}

/**
 * Указан не текстовой файл
 */
bool test_file_no_text ()
{
	/* Генерируем файл */
	char file[255];
	tmpnam (file);

	FILE * fp = fopen (file, "w");

	int size = 100;
	for (int i = 0; i < size; i++)
	{
		/* Вставляем нулёвые символы */
		if ((i % 10) == 0)
		{
			fputc (0, fp);
		}
		fputc ((int)random (), fp);
	}
	fclose (fp);

	/* Передаём функции */
	if (confi (file, confi_params_min) != CONFI_ERR_FILE_NO_TEXT)
	{
		return false;
	}

	/* Удаляем старый файл */
	remove (file);

	return true;
}

/**
 * Случайная строка
 */
bool test_file_random_string ()
{
	if (confi_parse_string ("random_string", confi_params_min) != CONFI_ERR_TOKEN_ORDER)
	{
		return false;
	}

	if (confi_parse_string ("param1 param2 param3", confi_params_min) != CONFI_ERR_TOKEN_ORDER)
	{
		return false;
	}

	if (confi_parse_string ("param=10", confi_params_min) != CONFI_ERR_TOKEN_ORDER)
	{
		return false;
	}

	return true;
}

/**
 * Пустой файл
 */
bool test_file_content_empty ()
{
	static struct confi_param confi_params_require[] =
	{
		{.name = "param", .type = CONFI_TYPE_INT, .require = true},
		NULL
	};

	if (confi_parse_string ("", confi_params_require) != CONFI_ERR_FILE_EMPTY_CONTENT)
	{
		return false;
	}

	if (confi_parse_string ("", confi_params_min) != 0)
	{
		return false;
	}

	return true;
}