#include "Vec.h"

#include <stdio.h>

void test_vec(void)
{
    vec_int_t v;

    vec_init(&v);
    vec_push(&v, 123);
    vec_push(&v, 456);

    printf("v[0]: %d\n", v.data[0]);
    printf("v[1]: %d\n", v.data[1]);

    //printf("v[2]: %d\n", v.data[2]);     // UB
    //printf("v[2]: %d\n", vec_at(v, 2));  // abort
}
