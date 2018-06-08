#pragma once

#include <stdbool.h>

/*** Данные и структуры ***/
/* Структура токенов */
struct token
{
	int type;
	char * content;
	struct token * next;
	bool param_isset;
};

/* Типы токенов */
enum token_type
{
	TOKEN_BLANK,							/* Код */
	TOKEN_WORD,								/* Слово */
	TOKEN_COMMENT,							/* Комментарий */
	TOKEN_EQUAL,							/* Знак равно */
	TOKEN_SEMICOLON,						/* Точка с запятой */
	TOKEN_STRING,							/* Строка */
	TOKEN_STRING_QUOTES_SINGLE,				/* Строка в одинарных кавычках */
	TOKEN_STRING_QUOTES_DOUBLE,				/* Строка в двойных кавычках */
	TOKEN_STRING_QUOTES_DOUBLE_ESCAPE		/* Символ экранирования */
};

/* Максимальные и минимальные значения */
enum
{
	TOKEN_BUF_MAX_SIZE = 1024
};

/*** Прототипы функций ***/
static struct token * token_parse_string (const char * str);

static int token_blank (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_comment (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_word (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_string_quotes_single (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_string_quotes_double (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_string_quotes_double_escape (char ch, char * buf, unsigned int * buf_size, struct token ** tokens);
static int token_equal (struct token ** tokens);
static int token_semicolon (struct token ** tokens);

static void token_push (int type, char * content, struct token ** tokens);

static int token_check_order (struct token * tokens);