#pragma once

#include <stdio.h>

#include "tests.h"

/**
 * Выполнение тестов
 */
int tests (char * name, struct test * tests)
{
	printf ("%s:\n", name);
	fflush (stdout);

	struct test * t = tests;
	while (t->name != NULL)
	{

		if (t->func())
		{
			printf ("\t- %s: Да.\n", t->name);
			fflush (stdout);
		}
		else
		{
			fprintf (stdout, "\t- %s: Нет.\n", t->name);
			fflush (stdout);
			return 1;
		}

		t++;
	}
	printf ("\n");

	return 0;
}