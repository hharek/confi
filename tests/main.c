#include <stdio.h>

#include "../confi.h"
#include "file.c"
#include "params.c"

int main ()
{
	/* Проверка файла */
	if (!test_file_not_found ())			{ fprintf (stderr, "test_file_not_found"); 		}
	if (!test_file_not_file ())				{ fprintf (stderr, "test_file_not_file"); 		}
	if (!test_file_empty ())				{ fprintf (stderr, "test_file_empty"); 			}
	if (!test_file_big_size ())				{ fprintf (stderr, "test_file_big_size"); 		}
	if (!test_file_not_file ())				{ fprintf (stderr, "test_file_not_file"); 		}
	if (!test_file_no_text ())				{ fprintf (stderr, "test_file_no_text"); 		}
	if (!test_file_random_string ())		{ fprintf (stderr, "test_file_random_string");	}
	if (!test_file_content_empty ())		{ fprintf (stderr, "test_file_content_empty");	}

	/* Параметры */
	if (!test_params_null ())				{ fprintf (stderr, "test_params_null"); 		}

	return 0;
}