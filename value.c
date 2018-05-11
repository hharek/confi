#pragma once

#include <stdlib.h>
#include <string.h>

#include "confi.h"
#include "value.h"
#include "error.h"
#include "error.c"

/**
 * Проверить строку на соответствие типу и назначить по адресу
 */
int value_set (struct confi_param * param)
{
	switch (param->type)
	{
		case CONFI_TYPE_INT:
			return value_set_int (param);

		case CONFI_TYPE_DOUBLE:
			return  value_set_double (param);

		case CONFI_TYPE_STRING:
			return  value_set_string (param);

		case CONFI_TYPE_BOOLEAN:
			return  value_set_bool (param);
	}
}

/**
 * Проверить и назначить как число со знаком
 */
int value_set_int (struct confi_param * param)
{
	char * end;
	strtol (param->value, &end, 0);
	if (*end)
	{
		error ("Параметр «%s» не является числом.", param->name);
		return -1;
	}

	if (param->ptr != NULL)
	{
		int value_int = (int)strtol (param->value, &end, 0);
		*((int *)param->ptr) = value_int;
	}

	return 0;
}

/**
 *  Проверить и назначить как число с плавающей точкой
 */
int value_set_double (struct confi_param * param)
{
	char * end;
	strtod (param->value, &end);
	if (*end)
	{
		error ("Параметр «%s» не является числом с плавающей запятой.", param->name);
		return -1;
	}

	if (param->ptr != NULL)
	{
		double value_double = strtod (param->value, &end);
		*((double *) param->ptr) = value_double;
	}

	return 0;
}

/**
 * Проверить и назначить как строку
 */
int value_set_string (struct confi_param * param)
{
	if (strlen (param->value) > 1024)
	{
		error ("Параметр «%s» содержит слишком длинную строку.", param->name);
		return -1;
	}

	if (param->ptr != NULL)
	{
		*((char **)param->ptr) = strdup (param->value);
	}

	return 0;
}

/**
 * Проверить и назначить как булёвое значение
 */
int value_set_bool (struct confi_param * param)
{
	if
	(
		strcmp (param->value, "1") != 0 &&
		strcmp (param->value, "0") != 0 &&
		strcmp (param->value, "true") != 0 &&
		strcmp (param->value, "false") != 0 &&
		strcmp (param->value, "yes") != 0 &&
		strcmp (param->value, "no") != 0 &&
		strcmp (param->value, "on") != 0 &&
		strcmp (param->value, "off") != 0
	)
	{
		error ("Параметр «%s» не является булёвым значением. Допустимые значения «1, 0, true, false, yes, no, on, off»", param->name);
		return -1;
	}

	if (param->ptr != NULL)
	{
		bool value_bool;
		if
		(
			strcmp (param->value, "1") == 0 ||
			strcmp (param->value, "true") == 0 ||
			strcmp (param->value, "yes") == 0 ||
			strcmp (param->value, "on") == 0)
		{
			value_bool = true;
		}
		else if
		(
			strcmp (param->value, "0") == 0 ||
			strcmp (param->value, "false") == 0 ||
			strcmp (param->value, "no") == 0 ||
			strcmp (param->value, "off") == 0
		)
		{
			value_bool = false;
		}

		*((bool *) param->ptr) = value_bool;
	}

	return 0;
}