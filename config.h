#include <stdbool.h>

/**
 * Типы параметров
 */
enum config_param_type
{
	CONFIG_PARAM_TYPE_UINT,
	CONFIG_PARAM_TYPE_DOUBLE,
	CONFIG_PARAM_TYPE_STRING,
	CONFIG_PARAM_TYPE_BOOLEAN
};

/**
 * Параметр
 */
struct config_param
{
	const char * name;
	enum config_param_type type;
	void * value;
};

/* Свой unsigned char */
typedef uint8_t uchar_t;

/**
 * Спарсить файл
 */
extern int32_t config_parse (const char * file, struct config_param * params, uint32_t params_size);

/**
 * Проверить имя параметра
 */
static struct config_param * get_config_param (uchar_t * param, struct config_param * params, uint32_t params_size);

