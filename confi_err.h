#include <stdio.h>
#include <stdarg.h>

/**
 * Назначить сообщение об ошибке
 */
static void error (const char * format, ... );

/**
 * Последнее сообщение об ошибке
 */
static char * err[1024];

/**
 * Вернуть последнее сообщение об ошибке
 */
const char * confi_err ()
{
	return err;
}

/**
 * Назначить сообщение об ошибке
 */
void error ( const char * format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf (err, format, args);

	va_end( args );
}