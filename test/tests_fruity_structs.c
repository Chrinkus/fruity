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

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(fruity_structs_new_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

