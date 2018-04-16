/**
 * Типы параметров
 */
enum confi_type
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
	enum confi_type type;
	void * value;
};

/**
 * Спарсить файл
 */
int confi_parse (const char * file, struct confi_param * params, unsigned int params_size);

/**
 * Показать сообщение об ошибке
 */
const char * confi_err ();
