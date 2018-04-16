#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "confi.h"
#include "confi_err.h"

/* Структура для токенов */
static struct token
{
	int type;
	char * content;
	struct token * next;
} * token_current, * tokens;

/*  Прототипы статических функций */
static void _token_push (int type, char * content);
static void _token_equal_push ();
static void _token_semicolon_push ();

static int _token_blank (char ch);
static int _token_comment (char ch);
static int _token_identified (char ch);
static int _token_string (char ch);

static bool _check_param (char * str);
static bool _check_value (char * str, enum confi_type type);

static struct confi_param * _get_param (char * param, struct confi_param * params, unsigned int params_size);

bool _value_set (struct confi_param * cparam,  char * value);

/* Типы токенов */
static enum
{
	TOKEN_BLANK,
	TOKEN_COMMENT,
	TOKEN_IDENTIFIED,
	TOKEN_EQUAL,
	TOKEN_STRING,
	TOKEN_SEMICOLON
};

int token = TOKEN_BLANK;

/* Максимальные и минимальные значения */
static enum
{
	BUF_MAX_SIZE = 1024
};

char buf[BUF_MAX_SIZE + 1] = {'\0'};
unsigned int buf_size = 0;

/* Строки */
static enum
{
	STRING_QUOTES_DOUBLE,
	STRING_QUOTES_SINGLE
};

static int string_type = STRING_QUOTES_DOUBLE;

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
		error ("Невозможно открыть файл «%s»", file);
		return -1;
	}

	/* Разбираем на токены */
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
			case TOKEN_BLANK:
			{
				token = _token_blank (ch);
			}
			break;

			/* Комментарий */
			case TOKEN_COMMENT:
			{
				token = _token_comment (ch);
			}
			break;

			/* Идентификатор */
			case TOKEN_IDENTIFIED:
			{
				token = _token_identified (ch);
			}
			break;

			/* Строка */
			case TOKEN_STRING:
			{
				token = _token_string (ch);
			}
			break;
		}
	}

	/* Проверяем порядок расположения токенов */
	struct token * i = tokens;
	while (i != NULL)
	{
		if
		(
			i->type != TOKEN_IDENTIFIED ||
			i->next->type != TOKEN_EQUAL ||
			(i->next->next->type != TOKEN_IDENTIFIED && i->next->next->type != TOKEN_STRING) ||
		   	i->next->next->next->type != TOKEN_SEMICOLON
		)
		{
			error ("Неверный порядок токенов");
			return -1;
		}

		i = i->next->next->next->next;
	}


	/* Разбираем токены */
	i = tokens;
	while (i != NULL)
	{
		static bool is_param = true;
		static char * param;
		static char * value;
		static struct confi_param * cparam;

		/* Параметр */
		if (TOKEN_IDENTIFIED && is_param)
		{
			param = i->content;
			is_param = false;

			if (!_check_param (param))
			{
				error ("Недопустимые символы в наименовании параметра «%s». Допускаются «a-z0-9_-»", param);
				return -1;
			}

			if ((cparam = _get_param (param, params, params_size)) == NULL)
			{
				error ("Неизвестный параметр «%s»", param);
				return -1;
			}
		}
		/* Значение */
		else if ( (TOKEN_IDENTIFIED && !is_param) || (TOKEN_STRING && !is_param) )
		{
			value = i->content;
			is_param = true;


			if (!_check_value (value, cparam->type))
			{
				error ("Значение параметра «%s» указано неверно.", param);
				return -1;
			}

//			cparam->value = i->content;
			_value_set (cparam, i->content);

		}

		i = i->next->next;
	}

	/* Закрыть файл */
    fclose (fp);

	return 0;
}

/**
 * Добавить токен
 */
void _token_push (int type, char * content)
{
	struct token * next = malloc (sizeof (struct token));
	next->type = type;
	next->content = content;

	if (!tokens || !token_current)
	{
		token_current = next;
		tokens = next;

		return;
	}

	token_current->next = next;
	token_current = next;
}

/**
 * Токен код
 */
int _token_blank (char ch)
{
	if (ch == '#')
	{
		return TOKEN_COMMENT;
	}
	else if (isblank (ch) || ch == '\r' || ch == '\n')
	{
		return TOKEN_BLANK;
	}
	else if (ch == '=')
	{
		_token_equal_push ();
		return TOKEN_BLANK;
	}
	else if (ch == ';')
	{
		_token_semicolon_push ();
		return TOKEN_BLANK;
	}
	else if (ch == '"')
	{
		string_type = STRING_QUOTES_DOUBLE;

		return TOKEN_STRING;
	}
	else if (ch == '\'')
	{
		string_type = STRING_QUOTES_SINGLE;

		return TOKEN_STRING;
	}
	else
	{
		buf[buf_size] = ch;
		buf_size++;

		return TOKEN_IDENTIFIED;
	}
}

/**
 * Токен «комментарий»
 */
int _token_comment (char ch)
{
	if (ch == '\n')
	{
		return TOKEN_BLANK;
	}
	else
	{
		return TOKEN_COMMENT;
	}
}

/**
 * Токен идентификатор
 */
int _token_identified (char ch)
{
	if (isblank (ch) || ch == '\r' || ch == '\n' || ch == '=' || ch == ';')
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		_token_push (TOKEN_IDENTIFIED, strdup (buf));

		if (ch == '=')
		{
			_token_equal_push ();
		}
		else if (ch == ';')
		{
			_token_semicolon_push ();
		}

		return TOKEN_BLANK;
	}
	else
	{
		buf[buf_size] = ch;
		buf_size++;

		return TOKEN_IDENTIFIED;
	}
}

/**
 * Токен «равно»
 */
void _token_equal_push ()
{
	buf[buf_size] = '\0';
	buf_size = 0;
	_token_push (TOKEN_EQUAL, strdup ("="));
}

/**
 * Токен «комментарий»
 */
int _token_string (char ch)
{
	if
	(
		(string_type == STRING_QUOTES_DOUBLE && ch == '"') ||
		(string_type == STRING_QUOTES_SINGLE && ch == '\'')
	)
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		_token_push (TOKEN_STRING, strdup (buf));

		return TOKEN_BLANK;
	}
	else
	{
		buf[buf_size] = ch;
		buf_size++;

		return TOKEN_STRING;
	}
}

/**
 * Токен «равно»
 */
void _token_semicolon_push ()
{
	buf[buf_size] = '\0';
	buf_size = 0;
	_token_push (TOKEN_SEMICOLON, strdup (";"));
}

/**
 * Проверка наименование параметра
 */
bool _check_param (char * str)
{
	/* Первый символ */
	char ch;
	ch = str[0];
	if (!isalpha (ch) && ch != '_')
	{
		return false;
	}
	str++;

	/* Остальные символы */
	while ((ch = str[0]) != 0)
	{
		if (!isalnum (ch) && ch != '_' && ch != '-')
		{
			return false;
		}
		str++;
	}

	return true;
}

/**
 * Проверка значения
 */
static bool _check_value (char * str, enum confi_type type)
{
	switch (type)
	{
		case CONFI_TYPE_INT:
		{
			char * end;
			strtol (str, &end, 0);
			if (*end)
			{
				return false;
			}
		}
		break;

		case CONFI_TYPE_DOUBLE:
		{
			char * end;
			strtod (str, &end);
			if (*end)
			{
				return false;
			}
		}
		break;

		case CONFI_TYPE_BOOLEAN:
		{
			if
			(
				strcmp (str, "1") != 0 &&
				strcmp (str, "0") != 0 &&
				strcmp (str, "true") != 0 &&
				strcmp (str, "false") != 0 &&
				strcmp (str, "yes") != 0 &&
				strcmp (str, "no") != 0 &&
				strcmp (str, "on") != 0 &&
				strcmp (str, "off") != 0
			)
			{
				return false;
			}
		}
		break;

		case CONFI_TYPE_STRING:
		{
			if (strlen (str) > 1024)
			{
				return false;
			}
		}
		break;
	}

	return true;
}

/**
 * Получить ссылку на параметр
 */
struct confi_param * _get_param (char * param, struct confi_param * params, unsigned int params_size)
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
 * Назначить значение
 */
bool _value_set (struct confi_param * cparam,  char * value)
{
	switch (cparam->type)
	{
		case CONFI_TYPE_INT:
		{
			char * end;
			int value_int = (int)strtol (value, &end, 0);

			cparam->value = malloc (sizeof (int));
			*((int *)cparam->value) = value_int;
		}
		break;

		case CONFI_TYPE_DOUBLE:
		{
			char * end;
			double value_double = strtod (value, &end);

			cparam->value = malloc (sizeof (double));
			*((double *)cparam->value) = value_double;
		}
		break;

		case CONFI_TYPE_BOOLEAN:
		{
			cparam->value = malloc (sizeof (bool));
			if (strcmp (value, "1") == 0 || strcmp (value, "true") == 0 || strcmp (value, "yes") == 0 || strcmp (value, "on") == 0)
			{
				*((bool *)cparam->value) = true;
			}
			else if (strcmp (value, "0") == 0 || strcmp (value, "false") == 0 || strcmp (value, "no") == 0 || strcmp (value, "off") == 0)
			{
				*((bool *)cparam->value) = false;
			}
		}
		break;

		case CONFI_TYPE_STRING:
		{
			cparam->value = (void *)strdup (value);
		}
		break;
	}

	return true;
}