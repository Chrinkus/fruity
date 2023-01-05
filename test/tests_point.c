#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include <stdio.h>

#include "fruity_point.h"

static void
cmp_2d_test(void** state)
{
        (void)state;
        int res = 1;

        // Equality test
        const struct f2d_point p0 = { .x = 0, .y = 0 };
        const struct f2d_point p1 = { .x = 0, .y = 0 };
        res = f2d_point_cmp(&p0, &p1);
        assert_true(res == 0);
        res = f2d_point_cmp(&p1, &p0);
        assert_true(res == 0);

        // Lesser X is lesser regardless of Y
        const struct f2d_point p2 = { .x = -5, .y = 2 };
        const struct f2d_point p3 = { .x = 5, .y = -2 };
        res = f2d_point_cmp(&p2, &p3);
        assert_true(res < 0);
        res = f2d_point_cmp(&p3, &p2);
        assert_true(res > 0);

        // Lesser Y is lesser when X's are the same
        const struct f2d_point p4 = { .x = 1, .y = -1 };
        const struct f2d_point p5 = { .x = 1, .y = 1 };
        res = f2d_point_cmp(&p4, &p5);
        assert_true(res < 0);
        res = f2d_point_cmp(&p5, &p4);
        assert_true(res > 0);
}

static void
hash_2d_test(void** state)
{
        (void)state;

        /*
        size_t b1 = 31;
        int a1[31] = { 0 };
        for (int i = -100; i <= 100; ++i)
                for (int j = -100; j <= 100; ++j) {
                        struct f2d_point p1 = { .x = j, .y = i };
                        ++a1[f2d_point_hash(p1, b1)];
                }
        for (int i = 0; i < 31; ++i)
                printf("%2d: %d\n", i, a1[i]);
        */

        size_t b2 = 31;
        size_t hash = 0;

        const struct f2d_point p1 = { .x = 0, .y = 0 };
        hash = f2d_point_hash(p1, b2);
        assert_true(hash < 31);

        const struct f2d_point p2 = { .x = 1, .y = 1 };
        hash = f2d_point_hash(p2, b2);
        assert_true(hash < 31);

        const struct f2d_point p3 = { .x = 1, .y = -1 };
        hash = f2d_point_hash(p3, b2);
        assert_true(hash < 31);

        const struct f2d_point p4 = { .x = -1, .y = 1 };
        hash = f2d_point_hash(p4, b2);
        assert_true(hash < 31);

        const struct f2d_point p5 = { .x = -1, .y = -1 };
        hash = f2d_point_hash(p5, b2);
        assert_true(hash < 31);
}

static void
move_2d_test(void** state)
{
        (void)state;

        struct f2d_point pt = { 0 };
        assert_int_equal(pt.x, 0);
        assert_int_equal(pt.y, 0);

        f2d_point_move(&pt, 1, -2);
        assert_int_equal(pt.x, 1);
        assert_int_equal(pt.y, -2);

        f2d_point_move(&pt, -5, 8);
        assert_int_equal(pt.x, -4);
        assert_int_equal(pt.y, 6);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(cmp_2d_test),
                cmocka_unit_test(hash_2d_test),
                cmocka_unit_test(move_2d_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
