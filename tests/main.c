#include <stdio.h>

#include "../confi.h"
#include "file.c"

int main ()
{
	if (!test_file_not_found ())			{ fprintf (stderr, "test_file_not_found"); 	}
	if (!test_file_not_file ())				{ fprintf (stderr, "test_file_not_file"); 	}
	if (!test_file_empty ())				{ fprintf (stderr, "test_file_empty"); 	}

	return 0;
}