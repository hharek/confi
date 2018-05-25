#include <stdio.h>
#include <stdlib.h>

#include "../confi.h"

static bool test_file_not_found ();
static bool test_file_not_file ();
static bool test_file_empty ();
static bool test_file_big_size ();
static bool test_file_no_text ();

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

bool test_file_big_size ()
{
	/* Генерируем большой файл */
	char file[255];
	tmpnam (file);

	FILE * fp = fopen (file, "w");

	int big_size = CONFI_FILE_MAX_SIZE + 1;
	for (int i = 0; i < big_size; i++)
	{
		fputc (random (), fp);
	}

	fclose (fp);

	/* Передаём функции */
	if (confi (file, NULL) != CONFI_ERR_FILE_MAX_SIZE)
	{
		return false;
	}

	/* Удаляем старый файл */
	remove (file);

	return true;
}

bool test_file_no_text ()
{


	return true;
}