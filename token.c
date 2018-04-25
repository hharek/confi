#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "token.h"

/**
 * Разбираем файл на токены
 */
struct token * token_parse_string (const char * str)
{
	struct token * tokens = malloc (sizeof (struct token));

	token_push (TOKEN_WORD, "param", &tokens);
	token_push (TOKEN_EQUAL, "=", &tokens);
	token_push (TOKEN_WORD, "100", &tokens);

//	char ch;
//	char * pos = (char *)str;
//
//	while ((ch = *(pos++)) != '\0')
//	{
//		switch (token_type)
//		{
//			/* Код */
//			case TOKEN_BLANK:
//			{
//				token_type = token_blank (ch, tokens);
//			}
//			break;
//
//			/* Комментарий */
//			case TOKEN_COMMENT:
//			{
//				token_type = token_comment (ch, tokens);
//			}
//			break;
//
//			/* Идентификатор */
//			case TOKEN_WORD:
//			{
//				token_type = token_word (ch, tokens);
//			}
//			break;
//
//			/* Строка */
//			case TOKEN_STRING:
//			{
//				token_type = token_string (ch, tokens);
//			}
//			break;
//
//			/* Знак «=» */
//			case TOKEN_EQUAL:
//			{
//				token_type = token_equal (ch, tokens);
//			}
//			break;
//
//			/* Знак «;» */
//			case TOKEN_SEMICOLON:
//			{
//				token_type = token_semicolon (ch, tokens);
//			}
//			break;
//		}
//	}

	return tokens;
}

/**
 * Токен «код»
 */
int token_blank (char ch, struct token * tokens)
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
		return TOKEN_EQUAL;
	}
	else if (ch == ';')
	{
		return TOKEN_SEMICOLON;
	}
	else if (ch == '"')
	{
		token_string_type = TOKEN_STRING_QUOTES_DOUBLE;

		return TOKEN_STRING;
	}
	else if (ch == '\'')
	{
		token_string_type = TOKEN_STRING_QUOTES_SINGLE;

		return TOKEN_STRING;
	}
	else
	{
		return token_word (ch, tokens);
	}
}

/**
 * Токен «комментарий»
 */
int token_comment (char ch, struct token * tokens)
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
 * Токен «слово»
 */
int token_word (char ch, struct token * tokens)
{
	static char         buf[TOKEN_BUF_MAX_SIZE + 1] = {'\0'};
	static unsigned int buf_size = 0;

	if (isblank (ch) || ch == '\r' || ch == '\n' || ch == '=' || ch == ';')
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		token_push (TOKEN_WORD, strdup (buf), &tokens);

		return token_blank (ch, tokens);
	}
	else
	{
		buf[buf_size] = ch;
		buf_size++;

		return TOKEN_WORD;
	}
}

/**
 * Токен «строка»
 */
int token_string (char ch, struct token * tokens)
{
	static char         buf[TOKEN_BUF_MAX_SIZE + 1] = {'\0'};
	static unsigned int buf_size = 0;

	if
	(
		(token_string_type == TOKEN_STRING_QUOTES_DOUBLE && ch == '"') ||
		(token_string_type == TOKEN_STRING_QUOTES_SINGLE && ch == '\'')
	)
	{
		buf[buf_size] = '\0';
		buf_size = 0;
		token_push (TOKEN_STRING, strdup (buf), &tokens);

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
 * Токен «=»
 */
int token_equal (char ch, struct token * tokens)
{
	token_push (TOKEN_EQUAL, strdup ("="), &tokens);

	return token_blank (ch, tokens);
}

/**
 * Токен «;»
 */
int token_semicolon (char ch, struct token * tokens)
{
	token_push (TOKEN_SEMICOLON, strdup (";"), &tokens);

	return token_blank (ch, tokens);
}

/**
 * Добавить токен
 */
void token_push (int type, char * content, struct token ** tokens)
{
	struct token * t = malloc (sizeof (struct token));
	t->type = type;
	t->content = content;
	t->first = (*tokens)->first;

	if ((*tokens)->first == NULL)
	{
		(*tokens)->first = t;
		*tokens = t;

		return;
	}

	(*tokens)->next = t;
	*tokens = t;
}

/* Проверяем порядок расположения токенов */
int token_check (struct token * tokens)
{
	struct token * i = tokens;
	while (i != NULL)
	{
		if
		(
			i->type != TOKEN_WORD ||
			i->next->type != TOKEN_EQUAL ||
			(i->next->next->type != TOKEN_WORD && i->next->next->type != TOKEN_STRING) ||
			i->next->next->next->type != TOKEN_SEMICOLON
		)
		{
			return -1;
		}

		i = i->next->next->next->next;
	}

	return 0;
}