#pragma once

#include <stdarg.h>
#include "confi.h"

/**
 * Последнее сообщение об ошибке
 */
static struct confi_err err;

/**
 * Назначить сообщение об ошибке
 */
static enum confi_err_code error (enum confi_err_code code, const char * message, const char * name);



