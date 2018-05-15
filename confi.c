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
#include "token.h"
#include "token.c"
#include "value.h"
#include "value.c"

/* Проверить конфигурационный файл */
static int confi_file_check (const char * file, FILE ** fp);

/* Проверить массив структур confi_params */
static int confi_params_check (struct confi_param * params);


/**
 * Спарсить файл
 */
int confi (const char * file, struct confi_param * params)
{
	FILE * fp;

	/* Проверить конфигурационный файл */
	int result = confi_file_check (file, &fp);
	if (result != 0)
	{
		return result;
	}

	/* Читаем файл */
	char content[CONFI_FILE_MAX_SIZE + 1];
	fread (content, CONFI_FILE_MAX_SIZE, 1, fp);

	/* Проверка параметров */
	if (confi_params_check (params) == -1)
	{
		return -4;
	}

	/* Парсим */
	if (confi_parse_string (content, params) == -1)
	{
		return -5;
	}

	/* Закрыть файл */
    fclose (fp);

	return 0;
}

/**
 * Проверить файл
 */
int confi_file_check (const char * file, FILE ** fp)
{
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
		error ("Невозможно открыть файл «%s».", file);
		return -11;
	}

	/* Статистика по файлу */
	struct stat st;
	fstat (fileno(*fp), &st);
	if (st.st_size > CONFI_FILE_MAX_SIZE)
	{
		error ("Размер файла не должен превышать «%d» байт.", CONFI_FILE_MAX_SIZE);
		return -12;
	}

	/* Бинарный файл */
	int ch;
	do
	{
		ch = fgetc(*fp);

		if (ch == '\0')
		{
			error ("Файл «%s» не является текстовым.", file);
			return -13;
		}
	}
	while (ch != EOF);
	fseek (*fp, 0, SEEK_SET);

	return 0;
}

/**
 * Спрасить строку
 */
int confi_parse_string (const char * str, struct confi_param * params)
{
	/* Разбираем строку на токены */
	struct token * tokens = token_parse_string (str);
	if (tokens == NULL)
	{
		return -1;
	}

	/* Заполняем значениями параметры */
	struct confi_param * param = params;
	struct token * t;
	while (param->name != NULL)
	{
		/* Назначаем параметрам значения с конфиг. файла */
		t = tokens;
		bool isset = false;
		while (t != NULL)
		{
			if (strcmp (param->name, t->content) == 0)
			{
				if (isset)
				{
					error ("Параметр «%s» повторяется.", param->name);
					return -1;
				}

				isset = true;
				param->value = strdup (t->next->next->content);
				t->param_isset = true;
			}

			t = t->next->next->next->next;
		}

		/* Обязательный параметр */
		if (param->require && !isset)
		{
			error ("Параметр «%s» является обязательным для заполнения.", param->name);
			return -1;
		}

		param++;
	}

	/* Неизвестные параметры */
	t = tokens;
	while (t != NULL)
	{
		if (!t->param_isset)
		{
			error ("Неизвестный параметр «%s».", t->content);
			return -1;
		}

		t = t->next->next->next->next;
	}

	/* Очищаем массив токенов */
	t = tokens;
	while (t != NULL)
	{
		void * old = (void *) t;
		t = t->next;
		free (old);
	}

	/* Проверяем и назначаем данные */
	param = params;
	while (param->name != NULL)
	{
		if (param->value != NULL)
		{
			if (value_set (param) == -1)
			{
				return -1;
			}
		}

		param++;
	}
}

/**
 * Вернуть последнее сообщение об ошибке
 */
char * confi_err ()
{
	return (char *)err;
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
			error ("Параметр «%s» имеет недопустимые символы. Название параметра может начинатся с символа: «a-z», «_»", param->name);
			return -1;
		}

		while ((ch = *(name++)) != '\0')
		{
			if (!isalnum (ch) && ch != '-' && ch != '_')
			{
				error ("Параметр «%s» имеет недопустимые символы. Допускается: «a-z», «-», «_»", param->name);
				return -1;
			}
		}

		param++;
	}

	return 0;
}