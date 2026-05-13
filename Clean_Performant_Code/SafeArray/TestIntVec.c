#include "IntVec.h"

#include <stdio.h>

void test_int_vec(void)
{
    int_vec v;

    int_vec_init(&v);
    int_vec_push(&v, 123);
    int_vec_push(&v, 456);

    printf("v[0]: %d\n", v.data[0]);
    printf("v[1]: %d\n", v.data[1]);

    int_vec_free(&v);
}
