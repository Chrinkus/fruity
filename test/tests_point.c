#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

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

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(cmp_2d_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
