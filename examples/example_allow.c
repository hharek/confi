#include <stdio.h>
#include <stdbool.h>

#include "../confi.h"

int main()
{
	/* Memory */
	struct
	{
		int param1; 	int param2;
		int param3;		int param4;
		int param5;

		int param_int;
		int param_int_plus;
		int param_int_minus;
		unsigned int param_uint;
		double param_double;
		double param_double_minus;
		double param_double_e;

		char * param_string_double;
		char * param_string_single;
		char * param_string_escape;
		char * param_string_newline;

		bool param_boolean_true;
		bool param_boolean_false;

		int param_default_int;
		char * param_default_string;
		bool param_default_bool;

		int param_repeat;
		int param_require;
	} params;

	/* My options */
	struct confi_param confi_params[] =
	{
		{ .name = "param1",		.type = CONFI_TYPE_INT,		.ptr = &params.param1 	},
		{ .name = "param2",		.type = CONFI_TYPE_INT,		.ptr = &params.param2 	},
		{ .name = "param3",		.type = CONFI_TYPE_INT,		.ptr = &params.param3 	},
		{ .name = "param4",		.type = CONFI_TYPE_INT,		.ptr = &params.param4 	},
		{ .name = "param5",		.type = CONFI_TYPE_INT,		.ptr = &params.param5 	},

		/* Numeric */
		{ .name = "param_int",			.type = CONFI_TYPE_INT,		.ptr = &params.param_int			},
		{ .name = "param_int_plus",		.type = CONFI_TYPE_INT,		.ptr = &params.param_int_plus		},
		{ .name = "param_int_minus",	.type = CONFI_TYPE_INT,		.ptr = &params.param_int_minus		},
		{ .name = "param_uint",			.type = CONFI_TYPE_UINT,	.ptr = &params.param_uint			},
		{ .name = "param_double",		.type = CONFI_TYPE_DOUBLE,	.ptr = &params.param_double			},
		{ .name = "param_double_minus",	.type = CONFI_TYPE_DOUBLE,	.ptr = &params.param_double_minus	},
		{ .name = "param_double_e",		.type = CONFI_TYPE_DOUBLE,	.ptr = &params.param_double_e		},

		/* String */
		{ .name = "param_string_double",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string_double	},
		{ .name = "param_string_single",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string_single	},
		{ .name = "param_string_escape",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string_escape	},
		{ .name = "param_string_newline",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_string_newline	},

		/* Boolean */
		{ .name = "param_boolean_true",		.type = CONFI_TYPE_BOOLEAN,	.ptr = &params.param_boolean_true	},
		{ .name = "param_boolean_false",	.type = CONFI_TYPE_BOOLEAN,	.ptr = &params.param_boolean_false	},

		/* Default */
		{ .name = "param_default_int",		.type = CONFI_TYPE_INT,		.ptr = &params.param_default_int,		.value = "100" 		},
		{ .name = "param_default_string",	.type = CONFI_TYPE_STRING,	.ptr = &params.param_default_string,	.value = "default" 	},
		{ .name = "param_default_bool",		.type = CONFI_TYPE_BOOLEAN,	.ptr = &params.param_default_bool,		.value = "true" 	},

		/* Other */
		{ .name = "param_repeat",	.type = CONFI_TYPE_INT,		.ptr = &params.param_repeat						},
		{ .name = "param_require",	.type = CONFI_TYPE_INT,		.ptr = &params.param_require,	.require = true	},

		NULL
	};

	/* Parsing */
	int result = confi ("example_allow.conf", confi_params);
	if (result != 0)
	{
		fprintf (stderr, confi_err ()->message);
		return 1;
	}

	/* Print */
	printf ("Format:\n");
	printf ("\tparam1 = %d\n", params.param1);
	printf ("\tparam2 = %d\n", params.param2);
	printf ("\tparam3 = %d\n", params.param3);
	printf ("\tparam4 = %d\n", params.param4);
	printf ("\tparam5 = %d\n", params.param5);
	printf ("\n");

	printf ("Numeric:\n");
	printf ("\tparam_int = %d\n", params.param_int);
	printf ("\tparam_int_plus = %d\n", params.param_int_plus);
	printf ("\tparam_int_minus = %d\n", params.param_int_minus);
	printf ("\tparam_uint = %u\n", params.param_uint);
	printf ("\tparam_double = %f\n", params.param_double);
	printf ("\tparam_double_minus = %f\n", params.param_double_minus);
	printf ("\tparam_double_e = %f\n", params.param_double_e);
	printf ("\n");

	printf ("String:\n");
	printf ("\tparam_string_double = %s\n", params.param_string_double);
	printf ("\tparam_string_single = %s\n", params.param_string_single);
	printf ("\tparam_string_escape = %s\n", params.param_string_escape);
	printf ("\tparam_string_newline = %s\n", params.param_string_newline);
	printf ("\n");

	printf ("Boolean:\n");
	printf ("\tparam_boolean_true = %d\n", params.param_boolean_true);
	printf ("\tparam_boolean_false = %d\n", params.param_boolean_false);
	printf ("\n");

	printf ("Default:\n");
	printf ("\tparam_default_int = %d\n", params.param_default_int);
	printf ("\tparam_default_string = %s\n", params.param_default_string);
	printf ("\tparam_default_bool = %d\n", params.param_default_bool);
	printf ("\n");

	printf ("Other:\n");
	printf ("\tparam_repeat = %d\n", params.param_repeat);
	printf ("\tparam_require = %d\n", params.param_require);
	printf ("\n");

    return 0;
}