#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "tests.h"
#include "tests.c"
#include "../confi.h"
#include "../inc/err.h"
#include "../inc/err.c"
#include "../inc/token.h"
#include "../inc/token.c"

static bool white_space ();					/* Проверка на пробельные символы */
static bool string_quote ();				/* Строки в одинарных и двойных кавычках */
static bool string_quote_escape ();			/* Экранирование строк в одинарных и двойных кавычках */
static bool word_big ();					/* Большое слово */
static bool string_big ();					/* Большая строка */

int main ()
{
	struct test token_tests[] =
	{
		{ .name = "white_space", 			.func = white_space 			},
		{ .name = "string_quote", 			.func = string_quote 			},
		{ .name = "string_quote_escape", 	.func = string_quote_escape 	},
		{ .name = "word_big", 				.func = word_big 				},
		{ .name = "string_big", 			.func = string_big 				},
		NULL
	};

	if (tests ("token.h", token_tests) != 0)
	{
		return 1;
	}

	return 0;
}

/**
 * Проверка на пробельные символы
 */
bool white_space ()
{
	const char * str =
		"param1 = 1;\n"
		"param2=2;\n"
		"     param3\t=\n\t3\n\n\r\n;\n"
		"param4 = 4;\r\n"
		"param5 = 5;param6 = 6;";

	struct token * tokens = token_parse_string (str);
	if (tokens == NULL)
	{
		return false;
	}

	struct token * t = tokens;

	if
	(
		strcmp (t->content, "param1") != 0 				|| t->type != TOKEN_WORD 						||
		strcmp (t->next->content, "=") != 0 			|| t->next->type != TOKEN_EQUAL 				||
		strcmp (t->next->next->content, "1") != 0		|| t->next->next->type != TOKEN_WORD			||
		strcmp (t->next->next->next->content, ";") != 0	|| t->next->next->next->type != TOKEN_SEMICOLON
	)
	{
		return false;
	}

	t = t->next->next->next->next;
	if
	(
		strcmp (t->content, "param2") != 0 				|| t->type != TOKEN_WORD 						||
		strcmp (t->next->content, "=") != 0 			|| t->next->type != TOKEN_EQUAL 				||
		strcmp (t->next->next->content, "2") != 0		|| t->next->next->type != TOKEN_WORD			||
		strcmp (t->next->next->next->content, ";") != 0	|| t->next->next->next->type != TOKEN_SEMICOLON
	)
	{
		return false;
	}

	t = t->next->next->next->next;
	if
	(
		strcmp (t->content, "param3") != 0 				|| t->type != TOKEN_WORD 						||
		strcmp (t->next->content, "=") != 0 			|| t->next->type != TOKEN_EQUAL 				||
		strcmp (t->next->next->content, "3") != 0		|| t->next->next->type != TOKEN_WORD			||
		strcmp (t->next->next->next->content, ";") != 0	|| t->next->next->next->type != TOKEN_SEMICOLON
	)
	{
		return false;
	}

	return true;
}

/**
 * Строки в одинарных и двойных кавычках
 */
bool string_quote ()
{
	const char * str =
		"param1 = \"str1\";"
		"param2 = 'str2';";

	struct token * tokens = token_parse_string (str);
	if (tokens == NULL)
	{
		return false;
	}

	struct token * t = tokens;

	if (strcmp (t->next->next->content, "str1") != 0 || t->next->next->type != TOKEN_STRING)
	{
		return false;
	}

	t = t->next->next->next->next;
	if (strcmp (t->next->next->content, "str2") != 0 || t->next->next->type != TOKEN_STRING)
	{
		return false;
	}

	return true;
}

/**
 * Экранирование строк в одинарных и двойных кавычках
 */
bool string_quote_escape ()
{
	const char * str =
		"param1 = \"str \\n str \\\" str ' str\";\n"  	/* param1 = "str \n str \" str ' str"; */
		"param2 = 'str \\n str \" str \\' str';";		/* param1 = 'str \n str " str \' str'; */

	struct token * tokens = token_parse_string (str);
	if (tokens == NULL)
	{
		return false;
	}

	struct token * t = tokens;

	/* param1 = "str \n str \" str ' str"; */
	if (strcmp (t->next->next->content, "str \n str \" str ' str") != 0 || t->next->next->type != TOKEN_STRING)
	{
		return false;
	}

	t = t->next->next->next->next;

	/* param1 = 'str \n str " str \' str'; */
	if (strcmp (t->next->next->content, "str \\n str \" str ' str") != 0 || t->next->next->type != TOKEN_STRING)
	{
		return false;
	}

	return true;
}

/**
 * Большое слово. Создаём строку типа aaa...aaa = 1;
 */
bool word_big ()
{
	char word[TOKEN_WORD_MAX_SIZE + 1];
	memset (word, 'a', TOKEN_WORD_MAX_SIZE + 1);
	word[TOKEN_WORD_MAX_SIZE + 1] = '\0';

	char str[TOKEN_WORD_MAX_SIZE + 1 + 6] = {'\0'};
	strcat (str, word);
	strcat (str, " = 1;");

	struct token * tokens = token_parse_string (str);
	if (tokens != NULL)
	{
		return false;
	}

	if (err_mess.code != CONFI_ERR_TOKEN_WORD_MAX_SIZE)
	{
		return false;
	}

	return true;
}

/**
 * Большая строка. Создаём строку типа param = "aaa...";
 */
bool string_big ()
{
	char value[TOKEN_STRING_MAX_SIZE + 3];
	memset (value, 'a', TOKEN_STRING_MAX_SIZE + 1);
	value[TOKEN_STRING_MAX_SIZE + 1] = '\0';

	char str_single[TOKEN_STRING_MAX_SIZE + 11] = "param = ";
	char str_double[TOKEN_STRING_MAX_SIZE + 11] = "param = ";

	strcat (str_single, "'");
	strcat (str_single, value);
	strcat (str_single, "';");

	strcat (str_double, "\"");
	strcat (str_double, value);
	strcat (str_double, "\";");

	if (token_parse_string (str_single) != NULL) 			{ return false; }
	if (err_mess.code != CONFI_ERR_TOKEN_STRING_MAX_SIZE) 	{ return false; }

	err_mess.code = CONFI_SUCCESS;

	if (token_parse_string (str_double) != NULL)			{ return false; }
	if (err_mess.code != CONFI_ERR_TOKEN_STRING_MAX_SIZE)	{ return false; }

	return true;
}