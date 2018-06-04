#include <stdio.h>
#include <stdlib.h>

#include "../confi.h"

static bool test_token_order ();

/**
 * Проверить порядок следования токенов
 */
bool test_token_order ()
{
	struct confi_param confi_params[] =
	{
		{.name = "param", .type = CONFI_TYPE_INT},
		NULL
	};

	if (confi_parse_string ("param1 param2 param3;", confi_params) != CONFI_ERR_TOKEN_ORDER)
	{
		return false;
	}

	if (confi_parse_string ("param", confi_params) != CONFI_ERR_TOKEN_ORDER)
	{
		return false;
	}

	return true;
}