#pragma once

/*** Данные и структуры ***/
/* Структура токенов */
static struct token
{
	int type;
	char * content;
	struct token * next;
};

/* Типы токенов */
static enum
{
	TOKEN_BLANK,
	TOKEN_WORD,
	TOKEN_COMMENT,
	TOKEN_EQUAL,
	TOKEN_STRING,
	TOKEN_SEMICOLON
};

static int token_type = TOKEN_BLANK;

/* Максимальные и минимальные значения */
static enum
{
	TOKEN_BUF_MAX_SIZE = 1024
};

/* Строки */
static enum
{
	TOKEN_STRING_QUOTES_DOUBLE,
	TOKEN_STRING_QUOTES_SINGLE
};

static int token_string_type = TOKEN_STRING_QUOTES_DOUBLE;

/*** Прототипы функций ***/
static struct token * token_parse_string (const char * str);

static  int token_blank (char ch, struct token ** tokens);
static  int token_comment (char ch, struct token ** tokens);
static  int token_word (char ch, struct token ** tokens);
static  int token_string (char ch, struct token ** tokens);
static  int token_equal (char ch, struct token ** tokens);
static  int token_semicolon (char ch, struct token ** tokens);

static void token_push (int type, char * content, struct token ** tokens);

static  int token_check_order (struct token * tokens);