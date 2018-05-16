#include <stdio.h>
#include "../confi.h"

static bool test_file_not_found ();
static bool test_file_not_file ();
static bool test_file_empty ();

bool test_file_not_found ()
{
	if (confi ("not_found", NULL) != CONFI_ERR_FILE_NOT_OPEN)
	{
		return false;
	}

	return true;
}

bool test_file_not_file ()
{
	if (confi (".", NULL) != CONFI_ERR_FILE_NOT_FILE)
	{
		return false;
	}

	return true;
}

bool test_file_empty ()
{
	if (confi ("", NULL) != CONFI_ERR_FILE_EMPTY)
	{
		return false;
	}

	return true;
}