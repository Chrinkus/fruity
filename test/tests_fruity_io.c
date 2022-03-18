#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "fruity_io.h"

void int_init_inc(Fruity2DMutable arr, int row, int col, void* data)
{
	int** ai = (int**)arr;
	int* val = (int*)data;

	ai[row][col] = (*val);

	*val += 10;
}

static void fruity_io_int_test(void** state)
{
	(void)state;

	int** ai = NULL;
	int rows = 5;
	int cols = 6;
	fruity_new(int, rows, cols, ai);
	assert_non_null(ai);

	int v = 0;
	fruity_transform(fruity_cast_mutable(ai), rows, cols, NULL,
			int_init_inc, &v);

	// No delimiter, no width, defaults to "touching"
	fruity_foreach(fruity_cast_const(ai), rows, cols,
			fruity_io_newline, fruity_io_print_int, NULL);

	// Delimiter indicated, should not have hanging delimiter
	fruity_io_delimiter = '-';
	fruity_foreach(fruity_cast_const(ai), rows, cols,
			fruity_io_newline, fruity_io_print_int, NULL);

	// Tests that the 'backspace' overwrite does not delete the last
	// element.
	fruity_io_delimiter = '\0';
	fruity_foreach(fruity_cast_const(ai), rows, cols,
			fruity_io_newline, fruity_io_print_int, NULL);

	// Recommended use: width set to expected size of output, delimiter
	// used to spread columns.
	int width = 3;
	fruity_io_delimiter = '\t';
	fruity_foreach(fruity_cast_const(ai), rows, cols,
			fruity_io_newline, fruity_io_print_int, &width);

	fruity_io_delimiter = FRUITY_IO_DELIMITER_DEFAULT;

	free(ai);
}

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(fruity_io_int_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}
