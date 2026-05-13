#include "IntVec.h"

#include <stdio.h>

void test_int_vec(void)
{
    int_vec vec;

    int_vec_init(&vec);
    int_vec_push(&vec, 123);
    int_vec_push(&vec, 456);
    int_vec_push(&vec, 789);

    printf("v[0]: %d\n", vec.data[0]);
    printf("v[1]: %d\n", vec.data[1]);
    printf("v[2]: %d\n", vec.data[2]);

    size_t i = 0;
    int* value;
    while ((value = int_vec_next(&vec, &i)) != NULL) {
        printf("%zu: %d\n", i, *value);
    }

    int_vec_free(&vec);
}
