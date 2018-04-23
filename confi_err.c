#include <stdio.h>
#include <stdarg.h>


static enum
{
	ERROR_MAX_SIZE = 1024
};

/**
 * Назначить сообщение об ошибке
 */
static void error (const char * format, ... );

/**
 * Последнее сообщение об ошибке
 */
static char * err[ERROR_MAX_SIZE];

/**
 * Вернуть последнее сообщение об ошибке
 */
const char * confi_err ()
{
	return (char *)err;
}

/**
 * Назначить сообщение об ошибке
 */
void error ( const char * format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf ((char *)err, format, args);

	va_end( args );
}