#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../tests.h"
#include "../tests.c"
#include "../../token.h"
#include "../../token.c"


static bool white_space ();					/* Проверка на пробельные символы */
static bool string_quote ();				/* Строки в одинарных и двойных кавычках */
static bool string_quote_escape ();			/* Экранирование строк в одинарных и двойных кавычках */

int main ()
{
	struct test token_tests[] =
	{
		{ .name = "white_space", 			.func = white_space },
		{ .name = "string_quote", 			.func = string_quote },
//		{ .name = "string_quote_escape", 	.func = string_quote_escape },
		NULL
	};

	if (tests (token_tests) != 0)
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
		"     param3\t=\n\t3\n\n\r\n;\n";

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

///**
// * Экранирование строк в одинарных и двойных кавычках
// */
//bool string_quote_escape ()
//{
//	return true;
//}