#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "fruity.h"

struct something {              // purposefully not "packed" well
        char id;
        double velocity;
        int count;
        struct something* next;
        unsigned flags;
};

static void fruity_structs_new_test(void** state)
{
        (void)state;

        Fruity2D fs = { 0 };
        fruity_new(struct something, 5, 13, &fs);

        struct something** p = fruity_data(&fs);

        assert_non_null(p);

        fruity_free(&fs);
}

static void fruity_structs_initialize_test(void** state)
{
        (void)state;

        Fruity2D fs = { 0 };
        fruity_new(struct something, 32, 8, &fs);

        struct something s = {
                .id = '$',
                .count = 42,
                .velocity = 1.0,
                .next = NULL,
                .flags = 1U << 1,
        };

        fruity_init(&fs, &s);

        struct something** p = fruity_data(&fs);

        assert_int_equal(p[0][0].id, '$');
        assert_int_equal(p[0][0].count, 42);
        assert_int_equal(p[0][0].flags, 2);
        assert_float_equal(p[0][0].velocity, 1.0, 0.1);
        assert_null(p[0][0].next);

        assert_int_equal(p[20][5].id, '$');
        assert_int_equal(p[20][5].count, 42);
        assert_int_equal(p[20][5].flags, 2);
        assert_float_equal(p[20][5].velocity, 1.0, 0.1);
        assert_null(p[20][5].next);

        assert_int_equal(p[31][7].id, '$');
        assert_int_equal(p[31][7].count, 42);
        assert_int_equal(p[31][7].flags, 2);
        assert_float_equal(p[31][7].velocity, 1.0, 0.1);
        assert_null(p[31][7].next);

        fruity_free(&fs);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_structs_new_test),
                cmocka_unit_test(fruity_structs_initialize_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

