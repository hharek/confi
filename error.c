#include <stdio.h>
#include <stdarg.h>

#include "error.h"

/**
 * Назначить сообщение об ошибке
 */
void error (char * format, ... )
{
	va_list args;
	va_start( args, format );
	vsprintf ((char *)err, format, args);

	va_end( args );
}