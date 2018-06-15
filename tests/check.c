#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "tests.h"
#include "tests.c"
#include "../err.h"
#include "../err.c"
#include "../check.h"
#include "../check.c"

static bool empty_string ();			/* Указана пустая строка или NULL */
static bool random_string ();			/* Случайная строка */
static bool big_file_name ();			/* Длинное название файла */
static bool current_dir ();				/* Указать вместо файла папку */
static bool file_big_size ();			/* Большой размер файла */
static bool file_binary ();				/* Бинарный файл */

int main ()
{
	struct test check_tests[] =
	{
		{ .name = "empty_string", 			.func = empty_string 	},
		{ .name = "random_string", 			.func = random_string 	},
		{ .name = "big_file_name", 			.func = big_file_name 	},
		{ .name = "current_dir", 			.func = current_dir 	},
		{ .name = "file_big_size", 			.func = file_big_size 	},
		{ .name = "file_binary", 			.func = file_binary 	},
		NULL
	};

	if (tests ("check.h", check_tests) != 0)
	{
		return 1;
	}

	return 0;
}

/**
 * Указана пустая строка или NULL
 */
bool empty_string ()
{
	FILE * fp;
	if (confi_file_check (NULL, &fp) != CONFI_ERR_FILE_EMPTY)
	{
		return false;
	}

	if (confi_file_check ("", &fp) != CONFI_ERR_FILE_EMPTY)
	{
		return false;
	}

	return true;
}

/**
 * Случайная строка
 */
bool random_string ()
{
	FILE * fp;
	if (confi_file_check ("asdfasdljlskdjflaskdjf123asdklfjkjskldfjalj123asijsdfseradfaelsjfeafsef", &fp) != CONFI_ERR_FILE_NOT_OPEN)
	{
		return false;
	}

	return true;
}

/**
 * Длинное имя файла
 */
bool big_file_name ()
{
	char file[256 + 1];
	memset (file, 'a', 256 + 1);
	file[256 + 1] = '\0';

	FILE * fp;
	if (confi_file_check (file, &fp) != CONFI_ERR_FILE_NAME_MAX_SIZE)
	{
		return false;
	}

	return true;
}

/**
 * Указать вместо файла папку
 */
bool current_dir ()
{
	FILE * fp;
	if (confi_file_check (".", &fp) != CONFI_ERR_FILE_NOT_FILE)
	{
		return false;
	}

	return true;
}

/**
 * Большой размер файла
 */
bool file_big_size ()
{
	/* Генерируем большой файл */
	char file[255];
	tmpnam (file);

	FILE * fp_tmp = fopen (file, "w");

	int big_size = CONFI_FILE_MAX_SIZE + 1;
	for (int i = 0; i < big_size; i++)
	{
		fputc ((int)random (), fp_tmp);
	}

	fclose (fp_tmp);

	/* Передаём функции */
	FILE * fp;
	if (confi_file_check (file, &fp) != CONFI_ERR_FILE_MAX_SIZE)
	{
		return false;
	}

	/* Удаляем старый файл */
	remove (file);

	return true;
}

/**
 * Бинарный файл
 */
bool file_binary ()
{
	/* Генерируем файл */
	char file[255];
	tmpnam (file);

	FILE * fp_tmp = fopen (file, "w");

	int size = 100;
	for (int i = 0; i < size; i++)
	{
		/* Вставляем нулёвые символы */
		if ((i % 10) == 0)
		{
			fputc (0, fp_tmp);
		}
		fputc ((int)random (), fp_tmp);
	}
	fclose (fp_tmp);

	/* Передаём функции */
	FILE * fp;
	if (confi_file_check (file, &fp) != CONFI_ERR_FILE_NO_TEXT)
	{
		return false;
	}

	/* Удаляем старый файл */
	remove (file);

	return true;
}