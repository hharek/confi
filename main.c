#include <stdio.h>
#include <stdbool.h>

#include "confi.h"

int main()
{
	const char * file = "main.conf";

	int param_int1;

	struct
	{
		int param_int1;
		int param_int2;
		double param_double1;
		double param_double2;
		int param_min;
		char * param_string1;
		char * param_string2;
		bool param_boolean1;
		bool param_boolean2;
		char * param_test;
	} params;

	struct confi_param confi_params[] =
	{
		{ .name = "param_int1",		.type = CONFI_TYPE_INT,		.ptr = &param_int1		},
		{ .name = "param_int2",		.type = CONFI_TYPE_INT 								},
		{ .name = "param_double1",	.type = CONFI_TYPE_DOUBLE 							},
		{ .name = "param_double2",	.type = CONFI_TYPE_DOUBLE,	.require = true			},
		{ .name = "param_min",		.type = CONFI_TYPE_INT 								},
		{ .name = "param_string1",	.type = CONFI_TYPE_STRING 							},
		{ .name = "param_string2",	.type = CONFI_TYPE_STRING 							},
		{ .name = "param_boolean1",	.type = CONFI_TYPE_BOOLEAN 							},
		{ .name = "param_boolean2",	.type = CONFI_TYPE_BOOLEAN 							},
		{ .name = "param_test",		.type = CONFI_TYPE_STRING,	.value = "test" 		},
		NULL
	};

	int result = confi (file, confi_params);
	if (result == -1)
	{
		fprintf (stderr, confi_err ());
		return 1;
	}


    return 0;
}