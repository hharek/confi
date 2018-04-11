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
static int _token_blank (char ch);
static int _token_comment (char ch);
static int _token_identified (char ch);
static int _token_string (char ch);


/* Типы токенов */
static enum
{
	TOKEN_BLANK,
	TOKEN_IDENTIFIED,
	TOKEN_EQUAL,
	TOKEN_STRING,
	TOKEN_COMMENT
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
static bool is_slash = false;

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
		return ERR_NOT_OPEN_FILE;
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

//	/* Разбираем параметры */
//	for (struct token * i = tokens; i != NULL; i = i->next)
//	{
//		printf ("%s\n", i->content);
//
//		switch (i->type)
//		{
//			case TOKEN_IDENTIFIED:
//			{
//
//			}
//			break;
//		}
//	}

	struct token * i = tokens;
	while (i != NULL)
	{
		printf ("%s=%s\n", i->content, i->next->next->content);

		i = i->next->next->next;
	}

	/* Закрыть файл */
    fclose (fp);

	return SUCCESS;
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
	else if (isblank (ch) || ch == '\n')
	{
		return TOKEN_BLANK;
	}
	else if (ch == '=')
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		_token_push (TOKEN_EQUAL, strdup ("="));

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
	if (isblank (ch) || ch == '\n' || ch == '=')
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		_token_push (TOKEN_IDENTIFIED, strdup (buf));

		if (ch == '=')
		{
			_token_push (TOKEN_EQUAL, strdup ("="));
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