#include <stdio.h>
#include "../confi.h"

static void test_file_not_found ();

void test_file_not_found ()
{
	struct confi_param confi_params[] =
	{
		{ .name = "param",	.type = CONFI_TYPE_INT },
		NULL
	};

	if (confi ("not_found", confi_params) == -1)
	{
		
	}
}