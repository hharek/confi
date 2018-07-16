#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#include "tests.h"
#include "tests.c"
#include "../err.h"
#include "../err.c"
#include "../value.h"
#include "../value.c"

static bool set_int ();
static bool set_uint ();
static bool set_double ();
static bool set_string ();
static bool set_bool ();

int main ()
{
	struct test value_tests[] =
	{
		{ .name = "set_int", 		.func = set_int 	},
		{ .name = "set_uint", 		.func = set_uint 	},
		{ .name = "set_double", 	.func = set_double 	},
		{ .name = "set_string", 	.func = set_string 	},
		{ .name = "set_bool", 		.func = set_bool 	},
		NULL
	};

	if (tests ("value.h", value_tests) != 0)
	{
		return 1;
	}

	return 0;
}

/**
 * Целое число со знаком
 */
bool set_int ()
{
	if (value_set_int ("param", "100", NULL) != 0)	 	{ return false; }
	if (value_set_int ("param", "+100", NULL) != 0)		{ return false; }
	if (value_set_int ("param", "-100", NULL) != 0)		{ return false; }

	if (value_set_int ("param", "abc", NULL) == 0)		{ return false; }
	if (value_set_int ("param", "0.1", NULL) == 0)		{ return false; }

	return true;
}

/**
 * Целое число без знака
 */
bool set_uint ()
{
	if (value_set_uint ("param", "100", NULL) != 0)		{ return false; }

	if (value_set_uint ("param", "+100", NULL) == 0)	{ return false; }
	if (value_set_uint ("param", "-100", NULL) == 0)	{ return false; }
	if (value_set_uint ("param", "0.1", NULL) == 0)		{ return false; }

	return true;
}

/**
 * Число с плавающей запятой со знаком
 */
bool set_double ()
{
	if (value_set_double ("param", "100", NULL) != 0)		{ return false; }
	if (value_set_double ("param", "100.01", NULL) != 0)	{ return false; }
	if (value_set_double ("param", "-100", NULL) != 0)		{ return false; }
	if (value_set_double ("param", "-100.01", NULL) != 0)	{ return false; }
	if (value_set_double ("param", "+100", NULL) != 0)		{ return false; }
	if (value_set_double ("param", "+100.01", NULL) != 0)	{ return false; }
	if (value_set_double ("param", "-1e-6", NULL) != 0)		{ return false; }
	if (value_set_double ("param", "0.1e6", NULL) != 0)		{ return false; }

	if (value_set_double ("param", "100.00.00", NULL) == 0)	{ return false; }
	if (value_set_double ("param", "abc", NULL) == 0)		{ return false; }

	return true;
}

/**
 * Строка
 */
bool set_string ()
{
	if (value_set_string ("param", "abc", NULL) != 0)		{ return false; }

	char big_string[CONFI_VALUE_STRING_MAX_LENGTH + 1];
	memset (big_string, 'a', CONFI_VALUE_STRING_MAX_LENGTH + 1);
	big_string[CONFI_VALUE_STRING_MAX_LENGTH + 1] = '\0';

	if (value_set_string ("param", big_string, NULL) == 0)	{ return false; }

	return true;
}

/**
 * Булёвое значение
 */
static bool set_bool ()
{
	if (value_set_bool ("param", "1", NULL) != 0)		{ return false; }
	if (value_set_bool ("param", "false", NULL) != 0)	{ return false; }
	if (value_set_bool ("param", "on", NULL) != 0)		{ return false; }
	if (value_set_bool ("param", "yes", NULL) != 0)		{ return false; }

	if (value_set_bool ("param", "11", NULL) == 0)		{ return false; }
	if (value_set_bool ("param", "TRUE", NULL) == 0)	{ return false; }
	if (value_set_bool ("param", "Yes", NULL) == 0)		{ return false; }

	return true;
}