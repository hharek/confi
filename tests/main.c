#include <stdio.h>

#include "../confi.h"
#include "file.c"
#include "param.c"
#include "token.c"
#include "value.c"

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
	if (!test_params_null ())					{ fprintf (stderr, "test_params_null"); 			}
	if (!test_params_name_first_bad_symbol())	{ fprintf (stderr, "test_params_first_bad_symbol"); }
	if (!test_params_name_bad_symbol())			{ fprintf (stderr, "test_params_name_bad_symbol");	}
	if (!test_params_repeat ())					{ fprintf (stderr, "test_params_repeat");			}
	if (!test_params_require ())				{ fprintf (stderr, "test_params_require");			}
	if (!test_params_unknown ())				{ fprintf (stderr, "test_params_unknown");			}

	/* Токены */
	if (!test_token_order ())				{ fprintf (stderr, "test_token_order");	}

	/* Значения */
	if (!test_value_not_int ())				{ fprintf (stderr, "test_value_not_int");		}
	if (!test_value_not_double ())			{ fprintf (stderr, "test_value_not_double");	}
	if (!test_value_not_boolean ())			{ fprintf (stderr, "test_value_not_boolean");	}
	if (!test_value_big_string ())			{ fprintf (stderr, "test_value_big_string");	}

	return 0;
}