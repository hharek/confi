#include <iostream>

extern "C"
{
	#include <confi.h>
}

int main ()
{
	struct
	{
		char * host;
		int port;
		bool ssl;
	} config;

	struct confi_param confi_params[] =
	{
		{ .name = "host",	.type = CONFI_TYPE_STRING, 	.value = nullptr,			.require = true,	.ptr = &config.host },
		{ .name = "port",	.type = CONFI_TYPE_INT, 	.value = (char *)"3333",	.require = false,	.ptr = &config.port	},
		{ .name = "ssl",	.type = CONFI_TYPE_BOOLEAN, .value = (char *)"false",	.require = false,	.ptr = &config.ssl	},
		nullptr
	};

	int result = confi ("example.conf", confi_params);
	if (result != 0)
	{
		std::cerr << confi_err ()->message << std::endl;
		return 1;
	}

	std::cout << "host = " << config.host << std::endl;
	std::cout << "port = " << config.port << std::endl;
	std::cout << "ssl = " << config.ssl << std::endl;

	return 0;
}