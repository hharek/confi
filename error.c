#pragma once

#include <stdio.h>
#include <string.h>

#include "confi.h"
#include "error.h"

/**
 * Назначить сообщение об ошибке
 */
enum confi_err_code error (enum confi_err_code code, const char * message, const char * name)
{
	err.code = code;

	if (name != NULL)
	{
		strncpy (err.name, name, 1024);
		sprintf (err.message, message, name);
	}
	else
	{
		strncpy (err.message, message, 2048);
	}


	return code;
}