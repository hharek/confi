#include <stdio.h>

#include "../../confi.h"
#include "file.c"
#include "param.c"
#include "token.c"
#include "value.c"

/**
 * Структура для тестов
 */
struct itest
{
	char * name;
	bool (*func) ();
};

/**
 * Выполнение интеграционных тестов
 */
void integration_tests (struct itest * tests);

int main ()
{
	struct itest tests[] =
	{
		/* Проверка файла */
		{ .name = "file_not_found", 				.func = test_file_not_found 				},
		{ .name = "file_not_file", 					.func = test_file_not_file 					},
		{ .name = "file_empty", 					.func = test_file_empty 					},
		{ .name = "file_big_size", 					.func = test_file_big_size 					},
		{ .name = "file_not_file", 					.func = test_file_not_file 					},
		{ .name = "file_no_text", 					.func = test_file_no_text 					},
		{ .name = "file_random_string", 			.func = test_file_random_string				},
		{ .name = "file_content_empty", 			.func = test_file_content_empty 			},

		/* Параметры */
		{ .name = "params_null", 					.func = test_params_null 					},
		{ .name = "params_name_first_bad_symbol", 	.func = test_params_name_first_bad_symbol 	},
		{ .name = "params_name_bad_symbol", 		.func = test_params_name_bad_symbol 		},
		{ .name = "params_repeat", 					.func = test_params_repeat					},
		{ .name = "params_require", 				.func = test_params_require 				},
		{ .name = "params_unknown", 				.func = test_params_unknown 				},

		/* Токены */
		{ .name = "token_order", 					.func = test_token_order 					},

		/* Значения */
		{ .name = "value_not_int", 					.func = test_value_not_int 					},
		{ .name = "value_not_double", 				.func = test_value_not_double 				},
		{ .name = "value_not_boolean", 				.func = test_value_not_boolean 				},
		{ .name = "value_big_string", 				.func = test_value_big_string 				},
		NULL
	};

	/* Запускаем тесты */
	integration_tests (tests);

	return 0;
}


void integration_tests (struct itest * tests)
{
	struct itest * t = tests;
	while (t->name != NULL)
	{

		if (t->func())
		{
			printf ("%s: Да.\n", t->name);
			fflush (stdout);
		}
		else
		{
			fprintf (stderr, "%s: Нет.\n", t->name);
			fflush (stderr);
		}

		t++;
	}
}