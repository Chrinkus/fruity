#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "fruity_io.h"

void int_init_inc(void* element, void* data)
{
        int* p = element;
        int* val = data;

        *p = *val;
        *val += 10;
}

static void fruity_io_int_test(void** state)
{
        (void)state;

        Fruity2D fi = { 0 };
        int rows = 5;
        int cols = 6;
        fruity_new(&fi, rows, cols, sizeof(int));
        assert_non_null(fruity_data(&fi));

        int v = 10;
        fruity_transform(&fi, NULL, NULL, int_init_inc, &v);

        // No delimiter, no width, defaults to "touching"
        fruity_foreach(&fi, fruity_io_newline, NULL, fruity_io_print_int, NULL);

        // Delimiter indicated, should not have hanging delimiter
        fruity_io_delimiter = '-';
        fruity_foreach(&fi, fruity_io_newline, NULL, fruity_io_print_int, NULL);

        // Tests that the 'backspace' overwrite does not delete the last
        // element.
        fruity_io_delimiter = '\0';
        fruity_foreach(&fi, fruity_io_newline, NULL, fruity_io_print_int, NULL);

        // Recommended use: width set to expected size of output, delimiter
        // used to spread columns.
        int width = 3;
        fruity_io_delimiter = '\t';
        fruity_foreach(&fi, fruity_io_newline, NULL, fruity_io_print_int,
                        &width);

        fruity_io_delimiter = FRUITY_IO_DELIMITER_DEFAULT;

        fruity_free(&fi);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_io_int_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

