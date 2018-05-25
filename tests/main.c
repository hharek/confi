#include <stdio.h>

#include "../confi.h"
#include "file.c"

int main ()
{
	if (!test_file_not_found ())			{ fprintf (stderr, "test_file_not_found"); 	}
	if (!test_file_not_file ())				{ fprintf (stderr, "test_file_not_file"); 	}
	if (!test_file_empty ())				{ fprintf (stderr, "test_file_empty"); 		}
	if (!test_file_big_size ())				{ fprintf (stderr, "test_file_big_size"); 	}
	if (!test_file_not_file ())				{ fprintf (stderr, "test_file_not_file"); 	}

	return 0;
}