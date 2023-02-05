#include "cmocka_headers.h"
#include <stdio.h>

#include "fruity_io.h"

const char* const good_read = "io_good_chars.txt";
const char* const good_line = "io_good_line.txt";
const char* const bad_chars = "io_bad_chars.txt";

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Setup and Teardown
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
static int
su_good_chars(void** state)
{
        FILE* file = fopen(good_read, "r");
        if (!file)
                return -1;

        *state = file;
        return 0;
}

static int
su_good_line(void** state)
{
        FILE* file = fopen(good_line, "r");
        if (!file)
                return -1;

        *state = file;
        return 0;
}

static int
su_bad_chars(void** state)
{
        FILE* file = fopen(bad_chars, "r");
        if (!file)
                return -1;

        *state = file;
        return 0;
}

static int
td_file(void** state)
{
        FILE* file = *state;
        fclose(file);
        return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Tests
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
static void
read_good_chars(void** state)
{
        FILE* file = *state;

        struct fruity_2d f2d = { 0 };
        assert_non_null(fruity_io_read_chars(&f2d, file));

        fruity_free(&f2d);
}

static void
read_good_line(void** state)
{
        FILE* file = *state;

        struct fruity_2d f2d = { 0 };
        assert_non_null(fruity_io_read_chars(&f2d, file));

        fruity_free(&f2d);
}

static void
read_bad_chars(void** state)
{
        FILE* file = *state;

        struct fruity_2d f2d = { 0 };
        assert_null(fruity_io_read_chars(&f2d, file));

        fruity_free(&f2d);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test_setup_teardown(read_good_chars,
                                su_good_chars, td_file),
                cmocka_unit_test_setup_teardown(read_good_line,
                                su_good_line, td_file),
                cmocka_unit_test_setup_teardown(read_bad_chars,
                                su_bad_chars, td_file),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
