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
        fruity_new(&fs, 5, 13, sizeof(struct something));

        struct something** p = fruity_data(&fs);

        assert_non_null(p);

        fruity_free(&fs);
}

static void fruity_structs_initialize_test(void** state)
{
        (void)state;

        Fruity2D fs = { 0 };
        fruity_new(&fs, 32, 8, sizeof(struct something));

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

static void inc_and_count(void* element, void* data)
{
        struct something* ps = element;
        int* pi = data;

        ++(*pi);
        ps->count = *pi;
}

static void fruity_structs_transform_test(void** state)
{
        (void)state;

        Fruity2D fs = { 0 };
        fruity_new(&fs, 5, 8, sizeof(struct something));

        struct something s = { 0 };
        fruity_init(&fs, &s);

        int count = 0;
        fruity_transform(&fs, NULL, NULL, inc_and_count, &count);

        struct something** p = fruity_data(&fs);
        assert_int_equal(p[0][0].count, 1);
        assert_int_equal(p[0][1].count, 2);
        assert_int_equal(p[1][7].count, 16);
        assert_int_equal(p[2][0].count, 17);
        assert_int_equal(p[4][6].count, 39);
        assert_int_equal(p[4][7].count, 40);

        fruity_free(&fs);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_structs_new_test),
                cmocka_unit_test(fruity_structs_initialize_test),
                cmocka_unit_test(fruity_structs_transform_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

