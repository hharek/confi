#include <stdio.h>
#include <stdbool.h>

#include "../confi.h"

int main()
{
	struct
	{
		int param_int1;
		int param_int2;
		double param_double1;
		double param_double2;
		int param_min;
		char * param_string1;
		char * param_string2;
		char * param_string3;
		bool param_boolean1;
		bool param_boolean2;
		char * param_default;
		int param_repeat;
	} params;

	struct confi_param confi_params[] =
	{
		{ .name = "param_int1",		.type = CONFI_TYPE_INT,		.ptr = &params.param_int1												},
		{ .name = "param_int2",		.type = CONFI_TYPE_INT, 	.ptr = &params.param_int2												},
		{ .name = "param_double1",	.type = CONFI_TYPE_DOUBLE,	.ptr = &params.param_double1 											},
		{ .name = "param_double2",	.type = CONFI_TYPE_DOUBLE,	.ptr = &params.param_double2,	.require = true							},
		{ .name = "param_min",		.type = CONFI_TYPE_INT,		.ptr = &params.param_min 												},
		{ .name = "param_string1",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string1											},
		{ .name = "param_string2",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string2 											},
		{ .name = "param_string3",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string3 											},
		{ .name = "param_boolean1",	.type = CONFI_TYPE_BOOLEAN,	.ptr = &params.param_boolean1 											},
		{ .name = "param_boolean2",	.type = CONFI_TYPE_BOOLEAN,	.ptr = &params.param_boolean2, 	.require = true							},
		{ .name = "param_default",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_default,					.value = "test_default" },
		{ .name = "param_repeat",	.type = CONFI_TYPE_INT,		.ptr = &params.param_repeat												},

		NULL
	};

	int result = confi ("main.conf", confi_params);
	if (result != 0)
	{
		fprintf (stderr, confi_err ()->message);
		return 1;
	}

	printf ("param_int1 = %d\n", params.param_int1);
	printf ("param_int2 = %d\n", params.param_int2);
	printf ("param_double1 = %f\n", params.param_double1);
	printf ("param_double2 = %f\n", params.param_double2);
	printf ("param_min = %d\n", params.param_min);
	printf ("param_string1 = %s\n", params.param_string1);
	printf ("param_string2 = %s\n", params.param_string2);
	printf ("param_string3 = %s\n", params.param_string3);
	printf ("param_boolean1 = %d\n", params.param_boolean1);
	printf ("param_boolean2 = %d\n", params.param_boolean2);
	printf ("param_default = %s\n", params.param_default);
	printf ("param_repeat = %d\n", params.param_repeat);

    return 0;
}