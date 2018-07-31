#include <stdio.h>
#include <stdbool.h>

#include "confi.h"

int main()
{
	struct
	{
		char * host;
		int port;
		bool ssl;
	} config;

	struct confi_param confi_params[] =
	{
		{ .name = "host",   .type = CONFI_TYPE_STRING,      .ptr = &config.host,                        .require = true },
		{ .name = "port",   .type = CONFI_TYPE_INT,         .ptr = &config.port,    .value = "3333"                     },
		{ .name = "ssl",    .type = CONFI_TYPE_BOOLEAN,     .ptr = &config.ssl,     .value = "false"                    },
		NULL
	};

	int result = confi ("example.conf", confi_params);
	if (result != 0)
	{
		fprintf (stderr, confi_err ()->message);
		return 1;
	}

	printf ("host = %s\n", config.host);
	printf ("port = %d\n", config.port);
	printf ("ssl = %d\n", config.ssl);

	return 0;
}