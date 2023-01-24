#include "cmocka_headers.h"

#include "fruity_set.h"

static void
set_new_test(void** state)
{
        (void)state;

        struct f2d_set fs = f2d_set_new();
        assert_int_equal(fs.length, 0);
        assert_int_equal(fs.capacity, 0);
        assert_null(fs.table);

        f2d_set_free(&fs);
}

static void
set_usage_test(void** state)
{
        (void)state;

        // create new set
        struct f2d_set fs = f2d_set_new();

        // search empty set
        const struct f2d_point p0 = { .x = 0, .y = 0 };
        assert_null(f2d_set_lookup(&fs, &p0));

        // insert point and try again
        assert_non_null(f2d_set_insert(&fs, &p0));
        assert_non_null(f2d_set_lookup(&fs, &p0));

        // insert a bunch of points (20 * 20 = 400)
        for (int i = -10; i < 10; ++i)
                for (int j = -10; j < 10; ++j) {
                        const struct f2d_point pt = { .x = j, .y = i };
                        assert_non_null(f2d_set_insert(&fs, &pt));
                }

        // check that 400 points were added
        assert_int_equal(f2d_set_length(&fs), 400);

        // check for elements in set
        const struct f2d_point p1 = { .x = -10, .y = 0 };
        assert_non_null(f2d_set_lookup(&fs, &p1));

        // check for elements not in set
        const struct f2d_point x1 = { .x = -10, .y = 10 };
        assert_null(f2d_set_lookup(&fs, &x1));

        // remove an element not in the set
        const struct f2d_point x2 = { .x = 13, .y = -2 };
        f2d_set_remove(&fs, &x2);

        assert_int_equal(f2d_set_length(&fs), 400);     // no change!

        // remove an element in the set
        const struct f2d_point p2 = { .x = -5, .y = -8 };
        assert_non_null(f2d_set_lookup(&fs, &p2));
        f2d_set_remove(&fs, &p2);
        assert_null(f2d_set_lookup(&fs, &p2));
        assert_int_equal(f2d_set_length(&fs), 399);     // element removed

        // insert an element already in the set
        const struct f2d_point p3 = { .x = 2, .y = -2 };
        assert_non_null(f2d_set_insert(&fs, &p3));
        assert_int_equal(f2d_set_length(&fs), 399);     // no change

        // free set
        f2d_set_free(&fs);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(set_new_test),
                cmocka_unit_test(set_usage_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
