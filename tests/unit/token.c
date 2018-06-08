#include <stdio.h>
#include <stdlib.h>

#include "../../token.h"
#include "../../token.c"

static bool test_unit_token_1 ();

int main ()
{
	if (!test_unit_token_1 ())		{ fprintf (stderr, "test_unit_token"); }

	return 0;
}

/**
 * Token test
 */
static bool test_unit_token_1 ()
{
	char str[255] = {'\0'};
	FILE * fp = fopen ("t1.conf", "r");
	fread (str, 1, 255, fp);

	struct token * tokens = token_parse_string (str);
	if (tokens == NULL)
	{
		printf ("%s\n", err_mess.message);
	}

	struct token * t = tokens;
	while (t != NULL)
	{
		printf ("%s\n", t->content);

		t = t->next;
	}

	return true;
}