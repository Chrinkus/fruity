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
        void* res = fruity_new(&ai, 10, 15, sizeof(int));
        // The array has "something" in it
        assert_non_null(res);
        assert_int_equal(ai.rows, 10);
        assert_int_equal(ai.cols, 15);
        // Clean up allocation
        fruity_free(&ai);
        assert_null(ai.data);
        assert_int_equal(ai.rows, 0);
        assert_int_equal(ai.cols, 0);

        Fruity2D ad = { 0 };
        res = fruity_new(&ad, 3, 22, sizeof(double));
        assert_non_null(res);
        assert_int_equal(ad.rows, 3);
        assert_int_equal(ad.cols, 22);
        fruity_free(&ad);
        assert_null(ad.data);
        assert_int_equal(ad.rows, 0);
        assert_int_equal(ad.cols, 0);

        Fruity2D ac = { 0 };
        res = fruity_new(&ac, 1000, 1000, sizeof(char));
        assert_non_null(res);
        assert_int_equal(ac.rows, 1000);
        assert_int_equal(ac.cols, 1000);
        fruity_free(&ac);
        assert_null(ac.data);
        assert_int_equal(ac.rows, 0);
        assert_int_equal(ac.cols, 0);

        Fruity2D as = { 0 };
        res = fruity_new(&as, 20, 30, sizeof(struct plot));
        assert_non_null(res);
        assert_int_equal(as.rows, 20);
        assert_int_equal(as.cols, 30);
        fruity_free(&as);
        assert_null(as.data);
        assert_int_equal(as.rows, 0);
        assert_int_equal(as.cols, 0);
}

static void int_inc(Fruity2DMutableData arr, int r, int c, void* data)
{
        int** ai = (int**)arr;
        int* p = (int*)data;

        ai[r][c] = (*p)++;
}

static void char_inc(Fruity2DMutableData arr, int r, int c, void* data)
{
        char** ac = (char**)arr;
        char* p = (char*)data;

        ac[r][c] = (*p)++;
}

static void fruity_transform_test(void** state)
{
        (void)state;

        Fruity2D fi = { 0 };
        void* res = fruity_new(&fi, 10, 10, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, int_inc, &v);

        int** ppi = fruity_data(&fi);
        assert_int_equal(ppi[0][0], 1);
        assert_int_equal(ppi[1][0], 11);
        assert_int_equal(ppi[5][5], 56);
        assert_int_equal(ppi[9][9], 100);

        fruity_free(&fi);

        Fruity2D fc = { 0 };
        res = fruity_new(&fc, 2, 13, sizeof(char));
        assert_non_null(res);

        char ch = 'a';
        fruity_transform(&fc, NULL, char_inc, &ch);

        char** ppc = fruity_data(&fc);
        assert_int_equal(ppc[0][0], 'a');
        assert_int_equal(ppc[0][12], 'm');
        assert_int_equal(ppc[1][0], 'n');
        assert_int_equal(ppc[1][12], 'z');

        fruity_free(&fc);
}

void accumulate(Fruity2DConstData arr, int r, int c, void* data)
{
        const int*const*const ai = (const int*const*const)arr;
        int* p = (int*)data;

        *p += ai[r][c];
}

static void fruity_foreach_test(void** state)
{
        (void)state;

        Fruity2D fi = { 0 };
        void* res = fruity_new(&fi, 5, 2, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, int_inc, &v);

        int sum = 0;
        fruity_foreach(&fi, NULL, accumulate, &sum);

        assert_int_equal(sum, 55);

        fruity_free(&fi);
}

static void fruity_init_test(void** state)
{
        (void)state;

        Fruity2D fi = { 0 };
        void* res = fruity_new(&fi, 4, 10, sizeof(int));
        assert_non_null(res);

        int ival = 37;
        fruity_init(&fi, &ival, sizeof(ival));

        int** pi = fruity_data(&fi);
        assert_int_equal(pi[0][0], 37);
        assert_int_equal(pi[0][1], 37);
        assert_int_equal(pi[2][4], 37);
        assert_int_equal(pi[2][5], 37);
        assert_int_equal(pi[3][9], 37);

        fruity_free(&fi);

        Fruity2D fd = { 0 };
        fruity_new(&fd, 12, 20, sizeof(double));
        assert_non_null(res);

        double dval = 3.14159;
        fruity_init(&fd, &dval, sizeof(dval));

        double** pd = fruity_data(&fd);
        assert_float_equal(pd[0][0], 3.14159, 0.000001);
        assert_float_equal(pd[0][1], 3.14159, 0.000001);
        assert_float_equal(pd[5][6], 3.14159, 0.000001);
        assert_float_equal(pd[5][7], 3.14159, 0.000001);
        assert_float_equal(pd[11][18], 3.14159, 0.000001);
        assert_float_equal(pd[11][19], 3.14159, 0.000001);

        fruity_free(&fd);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_new_test),
                cmocka_unit_test(fruity_transform_test),
                cmocka_unit_test(fruity_foreach_test),
                cmocka_unit_test(fruity_init_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

