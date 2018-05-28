#include <stdio.h>
#include <stdlib.h>

#include "../confi.h"

static bool test_params_null ();

/**
 * Параметры не заданы
 */
bool test_params_null ()
{
	if (confi ("simple.conf", NULL) != CONFI_ERR_PARAMS_NULL)
	{
		return false;
	}

	return true;
}
