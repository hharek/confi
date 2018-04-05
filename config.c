#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <uchar.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"


/* Типы токенов */
static enum
{
	IS_CODE,							/* Код */
	IS_COMMENT, 						/* Комментарий */
	IS_PARAM,							/* Наименование параметра */
	IS_EQUAL,							/* Знак равно */
	IS_QUOTES_DOUBLE,
	IS_QUOTES_DOUBLE_SLASH,
	IS_QUOTES_SINGLE,
	IS_QUOTES_SINGLE_SLASH,
	IS_UINT,
	IS_DOUBLE,
	IS_BOOLEAN
} state;

/* Настройки по параметрам */
static enum
{
	PARAM_MAX_SIZE = 12,
	VALUE_MAX_SIZE = 10
};

static uint32_t param_size = 0;
static uchar_t param[PARAM_MAX_SIZE + 1] = {'\0'};

static struct config_param * cparam;

static uint32_t value_size = 0;
static uchar_t value[VALUE_MAX_SIZE] = {'\0'};

/**
 * Спарсить файл
 */
int32_t config_parse (const char * file, struct config_param * params, uint32_t params_size)
{
	char dir[1024];
	getcwd (dir, sizeof (dir));

	strcat (dir, "/");
	strcat (dir, file);

	/* Открыть файл */
    FILE * fp = fopen (file, "r");
	if (!fp)
	{
		return -1;
	}

	/* Посимвольный парсинг */
	uchar_t ch;
	while (true)
	{
		ch = (uchar_t)fgetc (fp);
		if (feof (fp))
		{
			break;
		}

		switch (state)
		{
			/* Код */
			case IS_CODE:
			{
				if (isblank (ch) || ch == '\n')
				{
					continue;
				}
				else if (ch == '#')
				{
					state = IS_COMMENT;
				}
				else if (isalpha (ch) || ch == '-' || ch == '_')
				{
					param[0] = ch;
					param_size++;
					state = IS_PARAM;
				}
			}
			break;

			/* Комментарий */
			case IS_COMMENT:
			{
				if (ch == '\n')
				{
					state = IS_CODE;
				}
			}
			break;

			/* Название параметра */
			case IS_PARAM:
			{
				if (isalnum (ch) || ch == '-' || ch == '_')
				{
					param[param_size] = ch;
					param_size++;
					continue;
				}
				else
				{
					param[param_size] = '\0';

					if ((cparam = get_config_param (param, params, params_size)) == NULL)
					{
						fprintf (stderr, "Неизвестный параметр «%s».", param);
						return -1;
					}
				}

				if (ch == '=')
				{
					state = IS_EQUAL;
				}
				else if (isblank (ch))
				{
					continue;
				}
				else
				{
					fprintf (stderr, "Ошибка парсинга после параметра «%s».", param);
					return -1;
				}

			}
			break;

			/* Знак равно */
			case IS_EQUAL:
			{
				if (isblank (ch))
				{
					continue;
				}
				else
				{
					switch (cparam->type)
					{
						/* Тип число */
						case CONFIG_PARAM_TYPE_UINT:
						{
							if (isdigit(ch))
							{
								state = IS_UINT;
								value[value_size] = ch;
								value_size++;
							}
							else
							{
								fprintf (stderr, "Параметр «%s» не является числом.", param);
								return -1;
							}
						}
						break;

						/* Число с плавающей точкой */
						case CONFIG_PARAM_TYPE_DOUBLE:
						{
							if (isdigit(ch))
							{
								state = IS_DOUBLE;
								value[value_size] = ch;
								value_size++;
							}
							else
							{
								fprintf (stderr, "Параметр «%s» не является числом с плавающей точкой.", param);
								return -1;
							}
						}
						break;

						/* Строка */
						case CONFIG_PARAM_TYPE_STRING:
						{
							if (ch == '"')
							{
								state = IS_QUOTES_DOUBLE;
							}
							else if (ch == '\'')
							{
								state = IS_QUOTES_SINGLE;
							}
							else
							{
								fprintf (stderr, "Параметр «%s» не является строкой.", param);
								return -1;
							}
						}
						break;

						/* Булёвое значение */
						case CONFIG_PARAM_TYPE_BOOLEAN:
						{
							if (isalnum (ch))
							{
								state = IS_BOOLEAN;
								value[value_size] = ch;
								value_size++;
							}
							else
							{
								fprintf (stderr, "Параметр «%s» не является булёвым значением.", param);
								return -1;
							}
						}
						break;
					}
				}
			}
			break;

			/* Число */
			case IS_UINT:
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
					int value_int = strtol (value, &end, 0);
					if (*end)
					{
						fprintf (stderr, "Параметр «%s» не является числом.", param);
						return -1;
					}

					cparam->value = malloc (sizeof (int));
					*((int *)cparam->value) = value_int;

//					printf ("Параметр «%s» = «%d».\n", param, *((int *)cparam->value));
					state = IS_CODE;

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
			case IS_DOUBLE:
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
					state = IS_CODE;

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
			case IS_QUOTES_DOUBLE:
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
					state = IS_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else if (ch == '\\')
				{
					state = IS_QUOTES_DOUBLE_SLASH;
				}
			}
			break;

			case IS_QUOTES_DOUBLE_SLASH:
			{
				value[value_size] = ch;
				value_size++;
				state = IS_QUOTES_DOUBLE;
			}
			break;

			/* Одинарные кавычки */
			case IS_QUOTES_SINGLE:
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
					state = IS_CODE;

					param_size = 0;
					memset (param, '\0', PARAM_MAX_SIZE);
					value_size = 0;
					memset (value, '\0', VALUE_MAX_SIZE);
				}
				else if (ch == '\\')
				{
					state = IS_QUOTES_SINGLE_SLASH;
				}
			}
			break;

			case IS_QUOTES_SINGLE_SLASH:
			{
				value[value_size] = ch;
				value_size++;
				state = IS_QUOTES_SINGLE;
			}
			break;

			case IS_BOOLEAN:
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
					state = IS_CODE;

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
		}
	}

	/* Файл заканчивается неправильно */
	if (state != IS_CODE)
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
static struct config_param * get_config_param (uchar_t * param, struct config_param * params, uint32_t params_size)
{
	for (uint32_t i = 0; i < params_size; i++)
	{
		if (strcmp (params[i].name, param) == 0)
		{
			return &params[i];
		}
	}

	return NULL;
}

