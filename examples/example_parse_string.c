#include <stdio.h>
#include <stdbool.h>

#include "confi.h"

int main()
{
	int param1;
	char * param2;
	bool param3;

	struct confi_param confi_params[] =
	{
		{ .name = "param1",		.type = CONFI_TYPE_INT,		.ptr = &param1 	},
		{ .name = "param2",		.type = CONFI_TYPE_STRING,  .ptr = &param2  },
		{ .name = "param3",		.type = CONFI_TYPE_BOOLEAN,	.ptr = &param3 	},
		NULL
	};

	const char * content =
		"param1 = 100;\n"
		"param2 = 'string';\n"
		"param3 = true;";

	int result = confi_parse_string (content, confi_params);
	if (result != 0)
	{
		fprintf (stderr, confi_err ()->message);
		return 1;
	}

	printf ("param1 = %d\n", param1);
	printf ("param2 = %s\n", param2);
	printf ("param3 = %d\n", param3);

	return 0;
}