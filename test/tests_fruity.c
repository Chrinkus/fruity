#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "fruity.h"

struct plot {
        char id;
        int weight;
};

static void
fruity_new_test(void** state)
{
        (void)state;

        // Create 2D array
        struct fruity_2d ai = { 0 };
        void* res = fruity_new(&ai, 10, 15, sizeof(int));
        // The array has "something" in it
        assert_non_null(res);
        assert_int_equal(ai.rows, 10);
        assert_int_equal(ai.cols, 15);
        assert_int_equal(ai.size, sizeof(int));
        // Clean up allocation
        fruity_free(&ai);
        assert_null(ai.data);
        assert_int_equal(ai.rows, 0);
        assert_int_equal(ai.cols, 0);
        assert_int_equal(ai.size, 0);

        struct fruity_2d ad = { 0 };
        res = fruity_new(&ad, 3, 22, sizeof(double));
        assert_non_null(res);
        assert_int_equal(ad.rows, 3);
        assert_int_equal(ad.cols, 22);
        fruity_free(&ad);
        assert_null(ad.data);
        assert_int_equal(ad.rows, 0);
        assert_int_equal(ad.cols, 0);

        struct fruity_2d ac = { 0 };
        res = fruity_new(&ac, 1000, 1000, sizeof(char));
        assert_non_null(res);
        assert_int_equal(ac.rows, 1000);
        assert_int_equal(ac.cols, 1000);
        fruity_free(&ac);
        assert_null(ac.data);
        assert_int_equal(ac.rows, 0);
        assert_int_equal(ac.cols, 0);

        struct fruity_2d as = { 0 };
        res = fruity_new(&as, 20, 30, sizeof(struct plot));
        assert_non_null(res);
        assert_int_equal(as.rows, 20);
        assert_int_equal(as.cols, 30);
        fruity_free(&as);
        assert_null(as.data);
        assert_int_equal(as.rows, 0);
        assert_int_equal(as.cols, 0);
}

static void
build_test(void** state)
{
        (void)state;

        struct fruity_2d f1 = { 0 };
        int x1 = 5;
        assert_non_null(fruity_build(&f1, 7, 13, &x1, sizeof(x1)));

        // No getters, just member access
        assert_int_equal(f1.rows, 7);
        assert_int_equal(f1.cols, 13);
        assert_int_equal(f1.size, sizeof(int));
        assert_non_null(f1.data);

        const int*const* p1 = (const int*const*)(f1.data);
        assert_int_equal(p1[0][0], 5);
        assert_int_equal(p1[1][0], 5);
        assert_int_equal(p1[6][11], 5);
        assert_int_equal(p1[6][12], 5);

        fruity_free(&f1);

        struct fruity_2d f2 = { 0 };
        assert_non_null(fruity_build(&f2, 5, 14, NULL, sizeof(int)));

        const int* const* p2 = fruity_data(&f2);
        assert_int_equal(p2[0][0], 0);
        assert_int_equal(p2[0][13], 0);
        assert_int_equal(p2[3][0], 0);
        assert_int_equal(p2[4][13], 0);

        fruity_free(&f2);
}

static void
int_inc(struct f2d_cell_mut cell, void* data)
{
        int* pi = cell.data;
        int* pd = data;

        *pi = (*pd)++;
}

static void
char_inc(struct f2d_cell_mut cell, void* data)
{
        char* pc = cell.data;
        char* pd = data;

        *pc = (*pd)++;
}

static void
fruity_transform_test(void** state)
{
        (void)state;

        struct fruity_2d fi = { 0 };
        void* res = fruity_new(&fi, 10, 10, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, NULL, int_inc, &v);

        int*const* ppi = fruity_data(&fi);
        assert_int_equal(ppi[0][0], 1);
        assert_int_equal(ppi[1][0], 11);
        assert_int_equal(ppi[5][5], 56);
        assert_int_equal(ppi[9][9], 100);

        const int* cpi = fruity_get(&fi, 9, 9);
        assert_int_equal(*cpi, 100);

        int* pi = fruity_get_mut(&fi, 5, 5);
        assert_int_equal(*pi, 56);

        fruity_free(&fi);

        struct fruity_2d fc = { 0 };
        res = fruity_new(&fc, 2, 13, sizeof(char));
        assert_non_null(res);

        char ch = 'a';
        fruity_transform(&fc, NULL, NULL, char_inc, &ch);

        const char*const* ppc = fruity_data(&fc);
        assert_int_equal(ppc[0][0], 'a');
        assert_int_equal(ppc[0][12], 'm');
        assert_int_equal(ppc[1][0], 'n');
        assert_int_equal(ppc[1][12], 'z');

        fruity_free(&fc);
}

void
accumulate(struct f2d_cell cell, void* data)
{
        const int* ele = cell.data;
        int* p = data;

        *p += *ele;
}

void
doubler(struct f2d_cell_mut cell, void* data)
{
        int* ele = cell.data;
        (void)data;

        *ele *= 2;
}

static void
fruity_foreach_test(void** state)
{
        (void)state;

        struct fruity_2d fi = { 0 };
        void* res = fruity_new(&fi, 5, 2, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, NULL, int_inc, &v);

        int sum = 0;
        fruity_foreach(&fi, NULL, NULL, accumulate, &sum);

        assert_int_equal(sum, 55);

        fruity_free(&fi);
}

static void
fruity_copy_test(void** state)
{
        (void)state;

        struct fruity_2d f1 = { 0 };
        fruity_new(&f1, 6, 9, sizeof(int));

        int v = 1;
        fruity_transform(&f1, NULL, NULL, int_inc, &v);

        struct fruity_2d f2 = { 0 };
        void* res = fruity_copy(&f1, &f2);
        assert_non_null(res);

        const int*const* pi1 = fruity_data(&f1);
        assert_non_null(pi1);
        const int*const* pi2 = fruity_data(&f2);
        assert_non_null(pi2);
        assert_ptr_not_equal(pi1, pi2);

        assert_int_equal(pi2[0][0], 1);
        assert_int_equal(pi2[0][8], 9);
        assert_int_equal(pi2[3][0], 28);
        assert_int_equal(pi2[5][8], 54);

        fruity_transform(&f1, NULL, NULL, doubler, NULL);
        assert_int_equal(pi1[0][0], 2);
        assert_int_equal(pi1[0][8], 18);
        assert_int_equal(pi1[3][0], 56);
        assert_int_equal(pi1[5][8], 108);

        assert_int_equal(pi2[0][0], 1);
        assert_int_equal(pi2[0][8], 9);
        assert_int_equal(pi2[3][0], 28);
        assert_int_equal(pi2[5][8], 54);

        fruity_free(&f1);
        fruity_free(&f2);
}

static void
fruity_init_test(void** state)
{
        (void)state;

        struct fruity_2d fi = { 0 };
        void* res = fruity_new(&fi, 4, 10, sizeof(int));
        assert_non_null(res);

        int ival = 37;
        fruity_init(&fi, &ival);

        const int*const* ppi = fruity_data(&fi);
        assert_int_equal(ppi[0][0], 37);
        assert_int_equal(ppi[0][1], 37);
        assert_int_equal(ppi[2][4], 37);
        assert_int_equal(ppi[2][5], 37);
        assert_int_equal(ppi[3][9], 37);

        fruity_free(&fi);

        struct fruity_2d fd = { 0 };
        fruity_new(&fd, 12, 20, sizeof(double));
        assert_non_null(res);

        double dval = 3.14159;
        fruity_init(&fd, &dval);

        const double*const* pd = fruity_data(&fd);
        assert_float_equal(pd[0][0], 3.14159, 0.000001);
        assert_float_equal(pd[0][1], 3.14159, 0.000001);
        assert_float_equal(pd[5][6], 3.14159, 0.000001);
        assert_float_equal(pd[5][7], 3.14159, 0.000001);
        assert_float_equal(pd[11][18], 3.14159, 0.000001);
        assert_float_equal(pd[11][19], 3.14159, 0.000001);

        fruity_free(&fd);
}

int
is_multiple_of(struct f2d_cell cell, void* data)
{
        const int* pi = cell.data;
        const int* mul = data;

        return *pi != 0 && *pi % *mul == 0;
}

int
is_greater_than(struct f2d_cell cell, void* data)
{
        const int* pi = cell.data;
        const int* than = data;

        return *pi > *than;
}

static void
fruity_count_if_test(void** state)
{
        (void)state;

        struct fruity_2d fi = { 0 };
        void* res = fruity_new(&fi, 10, 10, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, NULL, int_inc, &v);

        int factor = 2;
        int mul2 = fruity_count_if(&fi, is_multiple_of, &factor);
        assert_int_equal(mul2, 50);

        factor = 3;
        int mul3 = fruity_count_if(&fi, is_multiple_of, &factor);
        assert_int_equal(mul3, 33);

        factor = 100;
        int gt = fruity_count_if(&fi, is_greater_than, &factor);
        assert_int_equal(gt, 0);

        fruity_free(&fi);
}

static void
fruity_adjacent_4_test(void** state)
        // [1][2][3]
        // [4][5][6]
        // [7][8][9]
{
        (void)state;

        struct fruity_2d fi = { 0 };
        void* res = fruity_new(&fi, 3, 3, sizeof(int));
        assert_non_null(res);

        int v = 1;
        fruity_transform(&fi, NULL, NULL, int_inc, &v);

        struct f2d_cell adj[4] = { { 0 } };
        int n = fruity_adjacent_4(&fi, 0, 0, adj);
        assert_int_equal(n, 2);
        assert_int_equal(*(int*)adj[0].data, 2);
        assert_int_equal(*(int*)adj[1].data, 4);

        n = fruity_adjacent_4(&fi, 1, 2, adj);
        assert_int_equal(n, 3);
        assert_int_equal(*(int*)adj[0].data, 3);
        assert_int_equal(*(int*)adj[1].data, 9);
        assert_int_equal(*(int*)adj[2].data, 5);

        n = fruity_adjacent_4(&fi, 1, 1, adj);
        assert_int_equal(n, 4);
        assert_int_equal(*(int*)adj[0].data, 2);
        assert_int_equal(*(int*)adj[1].data, 6);
        assert_int_equal(*(int*)adj[2].data, 8);
        assert_int_equal(*(int*)adj[3].data, 4);

        n = fruity_adjacent_4(&fi, 2, 2, adj);
        assert_int_equal(n, 2);
        assert_int_equal(*(int*)adj[0].data, 6);
        assert_int_equal(*(int*)adj[1].data, 8);

        fruity_free(&fi);
}

static void
move_test(void** state)
{
        (void)state;

        struct fruity_2d f1 = { 0 };
        int x = 37;
        fruity_build(&f1, 4, 2, &x, sizeof(x));

        assert_non_null(f1.data);
        assert_int_equal(f1.rows, 4);
        assert_int_equal(f1.cols, 2);

        const int* const* aai = fruity_data(&f1);
        assert_int_equal(aai[0][0], 37);
        assert_int_equal(aai[0][1], 37);
        assert_int_equal(aai[2][0], 37);
        assert_int_equal(aai[3][1], 37);

        struct fruity_2d f2 = { 0 };
        fruity_build(&f2, 23, 10, NULL, sizeof(int));

        assert_non_null(f2.data);
        assert_int_equal(f2.rows, 23);
        assert_int_equal(f2.cols, 10);

        aai = fruity_data(&f2);
        assert_int_equal(aai[0][0], 0);
        assert_int_equal(aai[0][1], 0);
        assert_int_equal(aai[2][0], 0);
        assert_int_equal(aai[3][1], 0);

        fruity_move(&f1, &f2);
        assert_null(f1.data);
        assert_int_equal(f1.rows, 0);
        assert_int_equal(f1.cols, 0);

        assert_non_null(f2.data);
        assert_int_equal(f2.rows, 4);
        assert_int_equal(f2.cols, 2);

        aai = fruity_data(&f2);
        assert_int_equal(aai[0][0], 37);
        assert_int_equal(aai[0][1], 37);
        assert_int_equal(aai[2][0], 37);
        assert_int_equal(aai[3][1], 37);

        fruity_free(&f2);
}

static void
grow_test(void** state)
{
        (void)state;

        struct fruity_2d f1 = { 0 };
        fruity_new(&f1, 3, 3, sizeof(int));
        int v1 = 1;
        fruity_transform(&f1, NULL, NULL, int_inc, &v1);

        const int* const* aai = fruity_data(&f1);
        assert_int_equal(aai[0][0], 1);
        assert_int_equal(aai[1][1], 5);
        assert_int_equal(aai[2][0], 7);
        assert_int_equal(aai[2][2], 9);

        assert_null(fruity_grow(&f1, -1, 5, 0, 0, NULL));       // negative dim
        assert_null(fruity_grow(&f1, 2, 1, 0, 5, NULL));        // big shift

        assert_non_null(fruity_grow(&f1, 2, 2, 1, 1, NULL));    // grow & shift!
        assert_int_equal(f1.rows, 5);
        assert_int_equal(f1.cols, 5);

        aai = fruity_data(&f1);
        assert_int_equal(aai[0][0], 0);
        assert_int_equal(aai[1][1], 1);
        assert_int_equal(aai[2][2], 5);
        assert_int_equal(aai[3][1], 7);
        assert_int_equal(aai[3][3], 9);
        assert_int_equal(aai[4][4], 0);

        fruity_free(&f1);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_new_test),
                cmocka_unit_test(build_test),
                cmocka_unit_test(fruity_transform_test),
                cmocka_unit_test(fruity_foreach_test),
                cmocka_unit_test(fruity_copy_test),
                cmocka_unit_test(fruity_init_test),
                cmocka_unit_test(fruity_count_if_test),
                cmocka_unit_test(fruity_adjacent_4_test),
                cmocka_unit_test(move_test),
                cmocka_unit_test(grow_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

