#include <stdbool.h>

/**
 * Типы параметров
 */
enum confi_param_type
{
	CONFI_TYPE_INT,
	CONFI_TYPE_DOUBLE,
	CONFI_TYPE_STRING,
	CONFI_TYPE_BOOLEAN
};

/**
 * Параметр
 */
struct confi_param
{
	const char * name;
	enum confi_param_type type;
	void * value;
};

/**
 * Спарсить файл
 */
extern int confi_parse (const char * file, struct confi_param * params, unsigned int params_size);


/**
 * Проверить имя параметра
 */
static struct confi_param * _get_param (const char * param, struct confi_param * params, unsigned int params_size);

/* Токены */
static int _token_code (char ch);
static int _token_comment (char ch);
static int _token_param (char ch);
static int _token_equal (char ch);
static int _token_value (char ch);