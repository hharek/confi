#pragma once

#include <string.h>

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "confi.h"
#include "error.h"
#include "error.c"

/**
 * Проверить файл
 */
int confi_file_check (const char * file, FILE ** fp)
{
	/* Пустая строка */
	if (file == NULL || *file == '\0')
	{
		return error (CONFI_ERR_FILE_EMPTY, "Файл не указан", NULL);
	}

	/* Текушая папка */
	char path[1024] = {'\0'};
	if (file[0] != '/')
	{
		getcwd (path, sizeof (path));
		strcat (path, "/");
	}
	strcat (path, file);

	/* Открыть файл */
	*fp = fopen (path, "r");
	if (!*fp)
	{
		return error (CONFI_ERR_FILE_NOT_OPEN, "Невозможно открыть файл «%s».", file);
	}

	/* Статистика по файлу */
	struct stat st;
	fstat (fileno(*fp), &st);

	/* Указан не файл */
	if (!S_ISREG (st.st_mode))
	{
		return error (CONFI_ERR_FILE_NOT_FILE, "«%s» не является файлом.", file);
	}

	/* Превышение размера */
	if (st.st_size > CONFI_FILE_MAX_SIZE)
	{
		return error (CONFI_ERR_FILE_MAX_SIZE, "Размер файла не должен превышать «%d» байт.", (int)CONFI_FILE_MAX_SIZE);
	}

	/* Бинарный файл */
	int ch;
	do
	{
		ch = fgetc(*fp);

		if (ch == '\0')
		{
			return error (CONFI_ERR_FILE_NO_TEXT, "Файл «%s» не является текстовым.", file);
		}
	}
	while (ch != EOF);
	fseek (*fp, 0, SEEK_SET);

	return 0;
}

/**
 * Проверить массив структур confi_params
 */
int confi_params_check (struct confi_param * params)
{
	struct confi_param * param = params;
	while (param->name != NULL)
	{
		char * name = (char *)param->name;
		char ch = name[0];
		if (!isalpha (ch) && ch != '_')
		{
			return error (CONFI_ERR_PARAM_NAME_FIRST_BAD_SYMBOL, "Параметр «%s» имеет недопустимые символы. Название параметра может начинатся с символа: «a-z», «_»", param->name);
		}

		while ((ch = *(name++)) != '\0')
		{
			if (!isalnum (ch) && ch != '-' && ch != '_')
			{
				return error (CONFI_ERR_PARAM_NAME_BAD_SYMBOL, "Параметр «%s» имеет недопустимые символы. Допускается: «a-z», «-», «_»", param->name);
			}
		}

		param++;
	}

	return 0;
}