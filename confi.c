#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "confi.h"


/* Типы токенов */
static enum
{
	TOKEN_CODE,
	TOKEN_COMMENT,
	TOKEN_PARAM,
	TOKEN_EQUAL,
	TOKEN_VALUE,
	TOKEN_QUOTES_DOUBLE,
	TOKEN_QUOTES_DOUBLE_SLASH,
	TOKEN_QUOTES_SINGLE,
	TOKEN_QUOTES_SINGLE_SLASH,
	TOKEN_INT,
	TOKEN_DOUBLE,
	TOKEN_BOOLEAN
};

int token;

/* Максимальные и минимальные значения */
static enum
{
	PARAM_MAX_SIZE = 255,
	VALUE_MAX_SIZE = 1024
};

/* Буферы для хранения названия и значения параметров */
static unsigned int param_size = 0;
static char param[PARAM_MAX_SIZE + 1] = {'\0'};

static unsigned int value_size = 0;
static char value[VALUE_MAX_SIZE + 1] = {'\0'};

/* Ссылка на текущий параметр */
static struct confi_param * cparam;

/**
 * Спарсить файл
 */
int confi_parse (const char * file, struct confi_param * params, unsigned int params_size)
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
		return -1;
	}

	/* Посимвольный парсинг */
	char ch;
	while (true)
	{
		ch = (char)fgetc (fp);
		if (feof (fp))
		{
			break;
		}

		switch (token)
		{
			/* Код */
			case TOKEN_CODE:
			{
				token = _token_code (ch);
			}
			break;

			/* Комментарий */
			case TOKEN_COMMENT:
			{
				token = _token_comment (ch);
			}
			break;

			/* Название параметра */
			case TOKEN_PARAM:
			{
				token = _token_param (ch);

				if (token == TOKEN_EQUAL)
				{
					cparam = _get_param (param, params, params_size);
				}
			}
			break;

			/* Знак равно */
			case TOKEN_EQUAL:
			{
				token = _token_equal (ch);
			}
			break;

			/* Значение */
			case TOKEN_VALUE:
			{
				token = _token_value (ch);
			}
			break;

			/* Число */
			case TOKEN_INT:
			{
				if (isdigit (ch))
				{
					value[value_size] = ch;
					value_size++;
				}
				else if (isblank (ch) || ch == '\n')
				{
					value[value_size] = '\0';

					char * end;
					int value_int = (int)strtol (value, &end, 0);
					if (*end)
					{
						fprintf (stderr, "Параметр «%s» не является числом.", param);
						return -1;
					}

					cparam->value = malloc (sizeof (int));
					*((int *)cparam->value) = value_int;

//					printf ("Параметр «%s» = «%d».\n", param, *((int *)cparam->value));
					token = TOKEN_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else
				{
					fprintf (stderr, "Параметр «%s» не является числом.", param);
					return -1;
				}
			}
			break;

			/* Число с плавающей точкой  */
			case TOKEN_DOUBLE:
			{
				if (isdigit (ch) || ch == '.')
				{
					value[value_size] = ch;
					value_size++;
				}
				else if (isblank (ch) || ch == '\n')
				{
					value[value_size] = '\0';

					char * end;
					double value_double = strtod (value, &end);
					if (*end)
					{
						fprintf (stderr, "Параметр «%s» не является числом с плавающей точкой.", param);
						return -1;
					}

					cparam->value = malloc (sizeof (int));
					*((double *)cparam->value) = value_double;

//					printf ("Параметр «%s» = «%f».\n", param, *((double *)cparam->value));
					token = TOKEN_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else
				{
					fprintf (stderr, "Параметр «%s» не является числом с плавающей точкой.", param);
					return -1;
				}
			}
			break;

			/* Двойные кавычки */
			case TOKEN_QUOTES_DOUBLE:
			{
				if (ch != '"' && ch != '\\')
				{
					value[value_size] = ch;
					value_size++;
				}
				else if (ch == '"')
				{
					value[value_size] = '\0';

					cparam->value = strdup (value);

//					printf ("Параметр «%s» = «%s».\n", param, (char *)value);
					token = TOKEN_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else if (ch == '\\')
				{
					token = TOKEN_QUOTES_DOUBLE_SLASH;
				}
			}
			break;

			case TOKEN_QUOTES_DOUBLE_SLASH:
			{
				value[value_size] = ch;
				value_size++;
				token = TOKEN_QUOTES_DOUBLE;
			}
			break;

			/* Одинарные кавычки */
			case TOKEN_QUOTES_SINGLE:
			{
				if (ch != '\'' && ch != '\\')
				{
					value[value_size] = ch;
					value_size++;
				}
				else if (ch == '\'')
				{
					value[value_size] = '\0';

					cparam->value = strdup (value);

//					printf ("Параметр «%s» = «%s».\n", param, (char *)value);
					token = TOKEN_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else if (ch == '\\')
				{
					token = TOKEN_QUOTES_SINGLE_SLASH;
				}
			}
			break;

			case TOKEN_QUOTES_SINGLE_SLASH:
			{
				value[value_size] = ch;
				value_size++;
				token = TOKEN_QUOTES_SINGLE;
			}
			break;

			case TOKEN_BOOLEAN:
			{
				if (isalnum (ch))
				{
					value[value_size] = ch;
					value_size++;
				}
				else if (isblank (ch) || ch == '\n')
				{
					value[value_size] = '\0';

					if
					(
						strcmp (value, "1") != 0 &&
						strcmp (value, "0") != 0 &&
						strcmp (value, "true") != 0 &&
						strcmp (value, "false") != 0 &&
						strcmp (value, "yes") != 0 &&
						strcmp (value, "no") != 0 &&
						strcmp (value, "on") != 0 &&
						strcmp (value, "off") != 0
					)
					{
						fprintf (stderr, "Параметр «%s» не является булёвым значением.", param);
						return -1;
					}

					cparam->value = malloc (sizeof (bool));
					if (strcmp (value, "1") == 0 || strcmp (value, "true") == 0 || strcmp (value, "yes") == 0 || strcmp (value, "on") == 0)
					{
						*((bool *)cparam->value) = true;
					}
					else if (strcmp (value, "0") == 0 || strcmp (value, "false") == 0 || strcmp (value, "no") == 0 || strcmp (value, "off") == 0)
					{
						*((bool *)cparam->value) = false;
					}

//					printf ("Параметр «%s» = «%d».\n", param, *((bool *)cparam->value));
					token = TOKEN_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else
				{
					fprintf (stderr, "Параметр «%s» не является булёвым значением.", param);
					return -1;
				}
			}
			break;

			/* Ошибка в парсинге */
			default:
			{
				fprintf (stderr, "Ошибка при парсинге.");
				return -1;
			}
		}
	}

	/* Файл заканчивается неправильно */
	if (token != TOKEN_CODE)
	{
		fprintf (stderr, "Ошибка парсинга");
		return -1;
	}

	/* Закрыть файл */
    fclose (fp);

	return 0;
}

/**
 * Проверить наличие праметра
 */
struct confi_param * _get_param (const char * param, struct confi_param * params, unsigned int params_size)
{
	for (unsigned int i = 0; i < params_size; i++)
	{
		if (strcmp (params[i].name, param) == 0)
		{
			return &params[i];
		}
	}

	return NULL;
}

/**
 * Токен «код»
 */
int _token_code (char ch)
{
	if (isblank (ch) || ch == '\n')
	{
		return TOKEN_COMMENT;
	}
	else if (ch == '#')
	{
		return TOKEN_COMMENT;
	}
	else if (isalpha (ch) || ch == '_')
	{
		return TOKEN_PARAM;
	}
	else
	{
		return -1;
	}
}

/**
 * Токен «комментарий»
 */
int _token_comment (char ch)
{
	if (ch == '\n')
	{
		return TOKEN_CODE;
	}
}

/**
 * Токен параметр
 */
int _token_param (char ch)
{
	if (isalnum (ch) || ch == '-' || ch == '_')
	{
		param[param_size] = ch;
		param_size++;

		return TOKEN_PARAM;
	}
	else if (isblank (ch))
	{
		return TOKEN_EQUAL;
	}
	else
	{
		return -1;
	}


//	else
//	{
//		param[param_size] = '\0';
//
//		if ((cparam = _get_param (param, params, params_size)) == NULL)
//		{
//			fprintf (stderr, "Неизвестный параметр «%s».", param);
//			return -1;
//		}
//	}
//
//	if (ch == '=')
//	{
//		token = TOKEN_EQUAL;
//	}
//	else if (isblank (ch))
//	{
//		continue;
//	}
//	else
//	{
//		fprintf (stderr, "Ошибка парсинга после параметра «%s».", param);
//		return -1;
//	}
}

/**
 * Токен равно
 */
int _token_equal (char ch)
{
	if (isblank (ch))
	{
		return TOKEN_EQUAL;
	}
	else if (ch == "=")
	{
		return TOKEN_VALUE;
	}


//	else
//	{
//		switch (cparam->type)
//		{
//			/* Тип число */
//			case CONFI_TYPE_INT:
//			{
//				if (isdigit(ch) || ch == '-')
//				{
//					token = TOKEN_INT;
//					value[value_size] = ch;
//					value_size++;
//				}
//				else
//				{
//					fprintf (stderr, "Параметр «%s» не является числом.", param);
//					return -1;
//				}
//			}
//				break;
//
//				/* Число с плавающей точкой */
//			case CONFI_TYPE_DOUBLE:
//			{
//				if (isdigit(ch))
//				{
//					token = TOKEN_DOUBLE;
//					value[value_size] = ch;
//					value_size++;
//				}
//				else
//				{
//					fprintf (stderr, "Параметр «%s» не является числом с плавающей точкой.", param);
//					return -1;
//				}
//			}
//				break;
//
//				/* Строка */
//			case CONFI_TYPE_STRING:
//			{
//				if (ch == '"')
//				{
//					token = TOKEN_QUOTES_DOUBLE;
//				}
//				else if (ch == '\'')
//				{
//					token = TOKEN_QUOTES_SINGLE;
//				}
//				else
//				{
//					fprintf (stderr, "Параметр «%s» не является строкой.", param);
//					return -1;
//				}
//			}
//				break;
//
//				/* Булёвое значение */
//			case CONFI_TYPE_BOOLEAN:
//			{
//				if (isalnum (ch))
//				{
//					token = TOKEN_BOOLEAN;
//					value[value_size] = ch;
//					value_size++;
//				}
//				else
//				{
//					fprintf (stderr, "Параметр «%s» не является булёвым значением.", param);
//					return -1;
//				}
//			}
//				break;
//		}
//	}
}


/**
 * Токен значение
 */
int _token_value (char ch)
{

}