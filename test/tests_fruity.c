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
        Fruity2D ai = { 0 };
        fruity_new(int, 10, 15, &ai);
        // The array has "something" in it
        assert_non_null(ai.data);
        assert_int_equal(ai.rows, 10);
        assert_int_equal(ai.cols, 15);
        // Clean up allocation
        fruity_free(&ai);
        assert_null(ai.data);
        assert_int_equal(ai.rows, 0);
        assert_int_equal(ai.cols, 0);

        Fruity2D ad = { 0 };
        fruity_new(double, 3, 22, &ad);
        assert_non_null(ad.data);
        assert_int_equal(ad.rows, 3);
        assert_int_equal(ad.cols, 22);
        fruity_free(&ad);
        assert_null(ad.data);
        assert_int_equal(ad.rows, 0);
        assert_int_equal(ad.cols, 0);

        Fruity2D ac = { 0 };
        fruity_new(char, 1000, 1000, &ac);
        assert_non_null(ac.data);
        assert_int_equal(ac.rows, 1000);
        assert_int_equal(ac.cols, 1000);
        fruity_free(&ac);
        assert_null(ac.data);
        assert_int_equal(ac.rows, 0);
        assert_int_equal(ac.cols, 0);

        Fruity2D as = { 0 };
        fruity_new(struct plot, 20, 30, &as);
        assert_non_null(as.data);
        assert_int_equal(as.rows, 20);
        assert_int_equal(as.cols, 30);
        fruity_free(&as);
        assert_null(as.data);
        assert_int_equal(as.rows, 0);
        assert_int_equal(as.cols, 0);
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

        Fruity2D fi = { 0 };
        fruity_new(int, 10, 10, &fi);

        int v = 1;
        fruity_transform(&fi, NULL, int_inc, &v);

        int** ppi = fruity_data(&fi);
        assert_int_equal(ppi[0][0], 1);
        assert_int_equal(ppi[1][0], 11);
        assert_int_equal(ppi[5][5], 56);
        assert_int_equal(ppi[9][9], 100);

        fruity_free(&fi);

        Fruity2D fc = { 0 };
        fruity_new(char, 2, 13, &fc);

        char ch = 'a';
        fruity_transform(&fc, NULL, char_inc, &ch);

        char** ppc = fruity_data(&fc);
        assert_int_equal(ppc[0][0], 'a');
        assert_int_equal(ppc[0][12], 'm');
        assert_int_equal(ppc[1][0], 'n');
        assert_int_equal(ppc[1][12], 'z');

        fruity_free(&fc);
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

        Fruity2D fi = { 0 };
        fruity_new(int, 5, 2, &fi);

        int v = 1;
        fruity_transform(&fi, NULL, int_inc, &v);

        int sum = 0;
        fruity_foreach(&fi, NULL, accumulate, &sum);

        assert_int_equal(sum, 55);

        fruity_free(&fi);
}

static void fruity_initialize_test(void** state)
{
        (void)state;

        Fruity2D fi = { 0 };
        fruity_new(int, 4, 10, &fi);

        int val = 37;
        fruity_initialize(&fi, &val, sizeof(val));

        int** p = fruity_data(&fi);
        assert_int_equal(p[0][0], 37);
        assert_int_equal(p[0][1], 37);
        assert_int_equal(p[2][4], 37);
        assert_int_equal(p[2][5], 37);
        assert_int_equal(p[3][9], 37);

        fruity_free(&fi);
}

static void fruity_init_test(void** state)
{
        (void)state;

        Fruity2D fd = { 0 };
        fruity_new(double, 12, 20, &fd);

        double val = 3.14159;
        fruity_init(&fd, &val);

        double** p = fruity_data(&fd);
        assert_float_equal(p[0][0], 3.14159, 0.000001);
        assert_float_equal(p[0][1], 3.14159, 0.000001);
        assert_float_equal(p[5][6], 3.14159, 0.000001);
        assert_float_equal(p[5][7], 3.14159, 0.000001);
        assert_float_equal(p[11][18], 3.14159, 0.000001);
        assert_float_equal(p[11][19], 3.14159, 0.000001);

        fruity_free(&fd);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_new_test),
                cmocka_unit_test(fruity_transform_test),
                cmocka_unit_test(fruity_foreach_test),
                cmocka_unit_test(fruity_initialize_test),
                cmocka_unit_test(fruity_init_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

