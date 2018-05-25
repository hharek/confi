#pragma once

#include <stdio.h>
#include <string.h>

#include "confi.h"
#include "err.h"

/**
 * Назначить сообщение об ошибке
 */
enum confi_err_code err (enum confi_err_code code, const char * format, const char * param1, const char * param2)
{
	err_mess.code = code;

	if (param1 != NULL && param2 != NULL)
	{
		strncpy (err_mess.param[0], param1, 1024);
		strncpy (err_mess.param[1], param2, 1024);
		sprintf (err_mess.message, format, param1, param2);
	}
	else if (param1 != NULL && param2 == NULL)
	{
		strncpy (err_mess.param[0], param1, 1024);
		sprintf (err_mess.message, format, param1);
	}
	else
	{
		strncpy (err_mess.message, format, 2048);
	}


	return code;
}