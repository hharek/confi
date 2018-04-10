#include <stdio.h>

#include "confi.h"


int main()
{
	const char * file = "main.conf";
	unsigned  int params_size = 7;
	struct confi_param params[] =
	{
		{
			.name = "param_int",
			.type = CONFI_TYPE_INT
		},
		{
			.name = "param_double",
			.type = CONFI_TYPE_DOUBLE
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
			.name = "param_string3",
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
		perror ("Ошибка парсинга");
	}

//	for (unsigned int i = 0; i < params_size; i++)
//	{
//		switch (params[i].type)
//		{
//			case CONFI_TYPE_INT:
//				printf ("%s = %d\n", params[i].name, *((int *)params[i].value));
//				break;
//
//			case CONFI_TYPE_DOUBLE:
//				printf ("%s = %lf\n", params[i].name, *((double *)params[i].value));
//				break;
//
//			case CONFI_TYPE_STRING:
//				printf ("%s = %s\n", params[i].name, (char *)params[i].value);
//				break;
//
//			case CONFI_TYPE_BOOLEAN:
//				printf ("%s = %d\n", params[i].name, *((bool *)params[i].value));
//				break;
//		}
//	}

    return 0;
}