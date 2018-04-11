/* Прототип */
const char * confi_err (unsigned int code);

/* Сообщения об ошибке */
static enum
{
	SUCCESS,
	ERR_COMMON,
	ERR_NOT_OPEN_FILE
};

static const char * error_message[] =
{
	[SUCCESS] = "Success",
	[ERR_COMMON] = "Common error",
	[ERR_NOT_OPEN_FILE] = "Do not open the file"
};

/**
 * Показать сообщение об ошибке
 */
const char * confi_err (unsigned int code)
{
	if (code > ERR_NOT_OPEN_FILE)
	{
		return "Unknown error";
	}

	return error_message[code];
}