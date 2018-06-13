#pragma once

#include <stdio.h>

#include "tests.h"

/**
 * Выполнение тестов
 */
int tests (struct test * tests)
{
	struct test * t = tests;
	while (t->name != NULL)
	{

		if (t->func())
		{
			printf ("%s: Да.\n", t->name);
			fflush (stdout);
		}
		else
		{
			fprintf (stdout, "%s: Нет.\n", t->name);
			fflush (stdout);
			return 1;
		}

		t++;
	}

	return 0;
}