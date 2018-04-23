#include <stdio.h>
#include <stdbool.h>

#include "confi.h"

int main()
{
	const char * file = "main.conf";
	unsigned  int params_size = 9;
	struct confi_param params[] =
	{
		{
			.name = "param_int1",
			.type = CONFI_TYPE_INT
		},
		{
			.name = "param_int2",
			.type = CONFI_TYPE_INT
		},
		{
			.name = "param_double1",
			.type = CONFI_TYPE_DOUBLE
		},
		{
			.name = "param_double2",
			.type = CONFI_TYPE_DOUBLE
		},
		{
			.name = "param_min",
			.type = CONFI_TYPE_INT
		},
		{
			.name = "param_string1",
			.type = CONFI_TYPE_STRING
		},
		{
			.name = "param_string2",
			.type = CONFI_TYPE_STRING
		},
		{
			.name = "param_boolean1",
			.type = CONFI_TYPE_BOOLEAN
		},
		{
			.name = "param_boolean2",
			.type = CONFI_TYPE_BOOLEAN
		}
	};

	int result = confi_parse (file, params, params_size);
	if (result == -1)
	{
		fprintf (stderr, confi_err ());
		return -1;
	}

	for (unsigned int i = 0; i < params_size; i++)
	{
		switch (params[i].type)
		{
			case CONFI_TYPE_INT:
				printf ("%s = %d\n", params[i].name, *((int *)params[i].value));
				break;

			case CONFI_TYPE_DOUBLE:
				printf ("%s = %lf\n", params[i].name, *((double *)params[i].value));
				break;

			case CONFI_TYPE_STRING:
				printf ("%s = %s\n", params[i].name, (char *)params[i].value);
				break;

			case CONFI_TYPE_BOOLEAN:
				printf ("%s = %d\n", params[i].name, *((bool *)params[i].value));
				break;
		}
	}

    return 0;
}