#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../confi.h"

static bool test_value_not_int ();
static bool test_value_not_double ();
static bool test_value_not_boolean ();
static bool test_value_big_string ();

/**
 * Параметры не заданы
 */
bool test_value_not_int ()
{
	struct confi_param confi_params[] =
	{
		{ .name = "param", .type = CONFI_TYPE_INT	},
		NULL
	};

	if (confi_parse_string ("param = false;", confi_params) != CONFI_ERR_VALUE_NOT_INT)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param") != 0)
	{
		return false;
	}

	return true;
}

/**
 * Параметр не является числом с плавающей запятой
 */
bool test_value_not_double ()
{
	struct confi_param confi_params[] =
	{
		{ .name = "param", .type = CONFI_TYPE_DOUBLE	},
		NULL
	};

	if (confi_parse_string ("param = 'string';", confi_params) != CONFI_ERR_VALUE_NOT_DOUBLE)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param") != 0)
	{
		return false;
	}

	return true;
}



/**
 * Параметр не является булёвым значением
 */
bool test_value_not_boolean ()
{
	struct confi_param confi_params[] =
	{
		{ .name = "param", .type = CONFI_TYPE_BOOLEAN	},
		NULL
	};

	if (confi_parse_string ("param = 100;", confi_params) != CONFI_ERR_VALUE_NOT_BOOLEAN)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param") != 0)
	{
		return false;
	}

	return true;
}

/**
 * Параметр содержит очень длинную строку
 */
bool test_value_big_string ()
{
	/* Создаём строку типа param = "aaa...";*/
	char str[CONFI_VALUE_STRING_MAX_LENGTH + 10] = "param = ";
	char string[CONFI_VALUE_STRING_MAX_LENGTH + 1 + 1];

	memset (string, 'a', CONFI_VALUE_STRING_MAX_LENGTH + 1);
	string[CONFI_VALUE_STRING_MAX_LENGTH + 1] = '\0';

	strcat (str, "\"");
	strcat (str, string);
	strcat (str, "\";");

	/* Запускаем */
	struct confi_param confi_params[] =
	{
		{ .name = "param", .type = CONFI_TYPE_STRING	},
		NULL
	};

	if (confi_parse_string (str, confi_params) != CONFI_ERR_VALUE_BIG_STRING)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param") != 0)
	{
		return false;
	}

	return true;
}