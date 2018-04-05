#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <uchar.h>
#include <stdbool.h>

typedef uint8_t uchar_t;

#include "config.h"


int main()
{
	const char * file = "main.conf";
	struct config_param params[] =
	{
		{
			.name = "param_int",
			.type = CONFIG_PARAM_TYPE_UINT
		},
		{
			.name = "param_double",
			.type = CONFIG_PARAM_TYPE_DOUBLE
		},
		{
			.name = "param_string1",
			.type = CONFIG_PARAM_TYPE_STRING
		},
		{
			.name = "param_string2",
			.type = CONFIG_PARAM_TYPE_STRING
		},
		{
			.name = "param_string3",
			.type = CONFIG_PARAM_TYPE_STRING
		},
		{
			.name = "param_boolean1",
			.type = CONFIG_PARAM_TYPE_BOOLEAN
		},
		{
			.name = "param_boolean2",
			.type = CONFIG_PARAM_TYPE_BOOLEAN
		}
	};

	int32_t result = config_parse (file, params, 7);
	if (result == -1)
	{
		perror ("Ошибка парсинга");
	}

    return 0;
}