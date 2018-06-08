#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../confi.h"

static bool test_params_null ();
static bool test_params_name_first_bad_symbol ();
static bool test_params_name_bad_symbol ();
static bool test_params_repeat ();
static bool test_params_require ();
static bool test_params_unknown ();

/**
 * Параметры не заданы
 */
bool test_params_null ()
{
	if (confi_parse_string ("", NULL) != CONFI_ERR_PARAMS_NULL)
	{
		return false;
	}

	return true;
}

/**
 * В наименовании параметра в начале недопустимый символ
 */
bool test_params_name_first_bad_symbol ()
{
	struct confi_param confi_params[] =
	{
		{.name = "1param", .type = CONFI_TYPE_INT},
		NULL
	};

	if (confi_parse_string ("", confi_params) != CONFI_ERR_PARAM_NAME_FIRST_BAD_SYMBOL)
	{
		return false;
	}

	return true;
}

/**
 * В наименование параметра недопустимые символы
 */
bool test_params_name_bad_symbol ()
{
	struct confi_param confi_params[] =
	{
		{.name = "par#am", .type = CONFI_TYPE_INT},
		NULL
	};

	if (confi_parse_string ("", confi_params) != CONFI_ERR_PARAM_NAME_BAD_SYMBOL)
	{
		return false;
	}

	return true;
}

/**
 * Параметр повторяется
 */
bool test_params_repeat ()
{
	struct confi_param confi_params[] =
	{
		{.name = "param", .type = CONFI_TYPE_INT},
		NULL
	};

	const char * str =
			"param = 1;\n"
			"param = 2;";

	if (confi_parse_string (str, confi_params) != CONFI_ERR_PARAM_REPEAT)
	{
		return false;
	}

	return true;
}

/**
 * Не заполнен файл обязательный для заполнения
 */
bool test_params_require ()
{
	struct confi_param confi_params[] =
	{
		{.name = "param", 		.type = CONFI_TYPE_INT					},
		{.name = "param_req", 	.type = CONFI_TYPE_INT, .require = true	},
		NULL
	};

	const char * str =
			"param = 1;"
			"#param_req = 2;";

	if (confi_parse_string (str, confi_params) != CONFI_ERR_PARAM_REQUIRE)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param_req") != 0)
	{
		return false;
	}

	return true;
}

/**
 * Неизвестный параметр
 */
bool test_params_unknown ()
{
	struct confi_param confi_params[] =
	{
		{ .name = "param", 		.type = CONFI_TYPE_INT },
		NULL
	};

	const char * str = "param_unknown = 1;";

	if (confi_parse_string (str, confi_params) != CONFI_ERR_PARAM_UNKNOWN)
	{
		return false;
	}

	if (strcmp (confi_err ()->param, "param_unknown") != 0)
	{
		return false;
	}

	return true;
}