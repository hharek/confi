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

//static bool _check_param (char * str);
//static bool _check_value (char * str, enum confi_type type);
//
//static struct confi_param * _get_param (char * param, struct confi_param * params, unsigned int params_size);
//
//bool _value_set (struct confi_param * cparam,  char * value);


/**
 * Спарсить файл
 */
int confi (const char * file, struct confi_param * params)
{
	/* Текушая папка */
	char dir[1024] = {'\0'};
	if (file[0] != '/')
	{
		getcwd (dir, sizeof (dir));
		strcat (dir, "/");
	}
	strcat (dir, file);

	/* Открыть файл */
    FILE * fp = fopen (dir, "r");
	if (!fp)
	{
		error ("Невозможно открыть файл «%s».", file);
		return -1;
	}


	/* Проверка файла */
	struct stat st;
	fstat (fileno(fp), &st);
	if (st.st_size > CONFI_FILE_MAX_SIZE)
	{
		error ("Размер файла не должен превышать «%d» байт.", CONFI_FILE_MAX_SIZE);
		return -1;
	}

	char content[CONFI_FILE_MAX_SIZE + 1];
	fread (content, CONFI_FILE_MAX_SIZE, 1, fp);
//	if (strlen (content) != st.st_size)
//	{
//		error ("Файл не является текстовым.");
//		return -1;
//	}


	/* Парсим */
	if (confi_parse_string (content, params) == -1)
	{
		return -1;
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
	/* Разбираем файл на токены */
	struct token * tokens = token_parse_string (str);

	struct token * i = tokens;
	while (i != NULL)
	{
		printf ("%s\n", i->content);
		i = i->next;
	}

//	/* Проверяем порядок следования токенов */
//	if (token_check (tokens) == -1)
//	{
//		return -1;
//	}
//
//	/* Разбираем токены */
//	struct token * i = tokens;
//	while (i != NULL)
//	{
//		static bool is_param = true;
//		static char * param;
//		static char * value;
//		static struct confi_param * cparam;
//
//		/* Параметр */
//		if (TOKEN_WORD && is_param)
//		{
//			param = i->content;
//			is_param = false;
//
//			if (!_check_param (param))
//			{
//				error ("Недопустимые символы в наименовании параметра «%s». Допускаются «a-z0-9_-»", param);
//				return -1;
//			}
//
//			if ((cparam = _get_param (param, params, params_size)) == NULL)
//			{
//				error ("Неизвестный параметр «%s»", param);
//				return -1;
//			}
//		}
//		/* Значение */
//		else if ( (TOKEN_WORD && !is_param) || (TOKEN_STRING && !is_param) )
//		{
//			value = i->content;
//			is_param = true;
//
//			if (!_check_value (value, cparam->type))
//			{
//				error ("Значение параметра «%s» указано неверно.", param);
//				return -1;
//			}
//
//			_value_set (cparam, i->content);
//		}
//
//		i = i->next->next;
//	}
}


/**
 * Вернуть последнее сообщение об ошибке
 */
char * confi_err ()
{
	return (char *)err;
}



///**
// * Проверка наименование параметра
// */
//bool _check_param (char * str)
//{
//	/* Первый символ */
//	char ch;
//	ch = str[0];
//	if (!isalpha (ch) && ch != '_')
//	{
//		return false;
//	}
//	str++;
//
//	/* Остальные символы */
//	while ((ch = str[0]) != 0)
//	{
//		if (!isalnum (ch) && ch != '_' && ch != '-')
//		{
//			return false;
//		}
//		str++;
//	}
//
//	return true;
//}
//
///**
// * Проверка значения
// */
//static bool _check_value (char * str, enum confi_type type)
//{
//	switch (type)
//	{
//		case CONFI_TYPE_INT:
//		{
//			char * end;
//			strtol (str, &end, 0);
//			if (*end)
//			{
//				return false;
//			}
//		}
//		break;
//
//		case CONFI_TYPE_DOUBLE:
//		{
//			char * end;
//			strtod (str, &end);
//			if (*end)
//			{
//				return false;
//			}
//		}
//		break;
//
//		case CONFI_TYPE_BOOLEAN:
//		{
//			if
//			(
//				strcmp (str, "1") != 0 &&
//				strcmp (str, "0") != 0 &&
//				strcmp (str, "true") != 0 &&
//				strcmp (str, "false") != 0 &&
//				strcmp (str, "yes") != 0 &&
//				strcmp (str, "no") != 0 &&
//				strcmp (str, "on") != 0 &&
//				strcmp (str, "off") != 0
//			)
//			{
//				return false;
//			}
//		}
//		break;
//
//		case CONFI_TYPE_STRING:
//		{
//			if (strlen (str) > 1024)
//			{
//				return false;
//			}
//		}
//		break;
//	}
//
//	return true;
//}
//
///**
// * Получить ссылку на параметр
// */
//struct confi_param * _get_param (char * param, struct confi_param * params, unsigned int params_size)
//{
//	for (unsigned int i = 0; i < params_size; i++)
//	{
//		if (strcmp (params[i].name, param) == 0)
//		{
//			return &params[i];
//		}
//	}
//
//	return NULL;
//}
//
///**
// * Назначить значение
// */
//bool _value_set (struct confi_param * cparam,  char * value)
//{
//	switch (cparam->type)
//	{
//		case CONFI_TYPE_INT:
//		{
//			char * end;
//			int value_int = (int)strtol (value, &end, 0);
//
//			cparam->value = malloc (sizeof (int));
//			*((int *)cparam->value) = value_int;
//		}
//		break;
//
//		case CONFI_TYPE_DOUBLE:
//		{
//			char * end;
//			double value_double = strtod (value, &end);
//
//			cparam->value = malloc (sizeof (double));
//			*((double *)cparam->value) = value_double;
//		}
//		break;
//
//		case CONFI_TYPE_BOOLEAN:
//		{
//			cparam->value = malloc (sizeof (bool));
//			if (strcmp (value, "1") == 0 || strcmp (value, "true") == 0 || strcmp (value, "yes") == 0 || strcmp (value, "on") == 0)
//			{
//				*((bool *)cparam->value) = true;
//			}
//			else if (strcmp (value, "0") == 0 || strcmp (value, "false") == 0 || strcmp (value, "no") == 0 || strcmp (value, "off") == 0)
//			{
//				*((bool *)cparam->value) = false;
//			}
//		}
//		break;
//
//		case CONFI_TYPE_STRING:
//		{
//			cparam->value = (void *)strdup (value);
//		}
//		break;
//	}
//
//	return true;
//}