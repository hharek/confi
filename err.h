#pragma once

#include <stdarg.h>
#include "confi.h"

/**
 * Инициализируем глобальную переменную с сообщение о последней ошибке
 */
static struct confi_err err_mess =
{
	.code = CONFI_SUCCESS,
	.param = {'\0', '\0'},
	.message = {'\0'}
};

/**
 * Создать сообщение об ошибке
 */
static enum confi_err_code err (enum confi_err_code code, const char * format, const char * param1, const char * param2);



