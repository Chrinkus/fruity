#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "fruity.h"

struct plot {
	char id;
	int weight;
};

static void fruity_new_test(void** state)
{
	(void)state;

	// Create 2D array
	int** ai = NULL;
	fruity_new(int, 10, 15, ai);
	// The array has "something" in it
	assert_non_null(ai);
	// Clean up allocation
	fruity_free(ai);

	double** ad = NULL;
	fruity_new(double, 3, 22, ad);
	assert_non_null(ad);
	fruity_free(ad);

	char** ac = NULL;
	fruity_new(char, 1000, 1000, ac);
	assert_non_null(ac);
	fruity_free(ac);

	struct plot** as = NULL;
	fruity_new(struct plot, 20, 30, as);
	assert_non_null(as);
	fruity_free(as);
}

static void int_inc(Fruity2DMutable arr, int r, int c, void* data)
{
	int** ai = (int**)arr;
	int* p = (int*)data;

	ai[r][c] = (*p)++;
}

static void char_inc(Fruity2DMutable arr, int r, int c, void* data)
{
	char** ac = (char**)arr;
	char* p = (char*)data;

	ac[r][c] = (*p)++;
}

static void fruity_transform_test(void** state)
{
	(void)state;

	int** ai = NULL;
	fruity_new(int, 10, 10, ai);
	assert_non_null(ai);

	int v = 1;
	fruity_transform(fruity_cast_mutable(ai), 10, 10, NULL, int_inc, &v);
	assert_int_equal(ai[0][0], 1);
	assert_int_equal(ai[1][0], 11);
	assert_int_equal(ai[5][5], 56);
	assert_int_equal(ai[9][9], 100);

	fruity_free(ai);

	char** ac = NULL;
	fruity_new(char, 2, 13, ac);
	assert_non_null(ac);

	char ch = 'a';
	fruity_transform(fruity_cast_mutable(ac), 2, 13, NULL, char_inc, &ch);
	assert_int_equal(ac[0][0], 'a');
	assert_int_equal(ac[1][12], 'z');

	fruity_free(ac);
}

void accumulate(Fruity2DConst arr, int r, int c, void* data)
{
	const int*const*const ai = (const int*const*const)arr;
	int* p = (int*)data;

	*p += ai[r][c];
}

static void fruity_foreach_test(void** state)
{
	(void)state;

	int** ai = NULL;
	fruity_new(int, 5, 2, ai);
	int v = 1;
	fruity_transform(fruity_cast_mutable(ai), 5, 2, NULL, int_inc, &v);

	int sum = 0;
	fruity_foreach(fruity_cast_const(ai), 5, 2, NULL, accumulate, &sum);

	assert_int_equal(sum, 55);

	fruity_free(ai);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(fruity_new_test),
		cmocka_unit_test(fruity_transform_test),
		cmocka_unit_test(fruity_foreach_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

