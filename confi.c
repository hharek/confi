#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "confi.h"
#include "err.h"
#include "err.c"
#include "check.h"
#include "check.c"
#include "token.h"
#include "token.c"
#include "value.h"
#include "value.c"

/**
 * Спарсить файл
 */
int confi (const char * file, struct confi_param * params)
{
	FILE * fp;
	int result = 0;

	/* Проверить конфигурационный файл */
	result = confi_file_check (file, &fp);
	if (result != 0)
	{
		return result;
	}

	/* Читаем файл */
	char content[CONFI_FILE_MAX_SIZE + 1];
	fread (content, CONFI_FILE_MAX_SIZE, 1, fp);

	/* Проверка параметров */
	result = confi_params_check (params);
	if (result != 0)
	{
		return result;
	}

	/* Парсим */
	result = confi_parse_string (content, params);
	if (result != 0)
	{
		return result;
	}

	/* Закрыть файл */
    fclose (fp);

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
		return confi_err ()->code;
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
					return err (CONFI_ERR_PARAM_REPEAT, "Параметр «%s» повторяется.", param->name);
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
			return err (CONFI_ERR_PARAM_REQUIRE, "Параметр «%s» является обязательным для заполнения.", param->name);
		}

		param++;
	}

	/* Неизвестные параметры */
	t = tokens;
	while (t != NULL)
	{
		if (!t->param_isset)
		{
			return err (CONFI_ERR_PARAM_UNKNOWN, "Неизвестный параметр «%s».", t->content);
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
struct confi_err * confi_err ()
{
	return &err;
}