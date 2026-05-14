// ===========================================================================
// UnitTestIntVec.c // Small Safe Array Library
// ===========================================================================

#include "IntVec.h"

#include <stdio.h>
#include <string.h>

static int pass_count = 0;
static int fail_count = 0;

static void test_section(const char* desc);
static void test_assert_func(int cond, const char* cond_str, const char* file, int line);
static void test_print_res();

// Note: Stringification of '#cond':
// Stringification functions work exclusively within preprocessor macros
// to convert the code text into a string
#define  test_assert(cond)   test_assert_func((cond), #cond, __FILE__, __LINE__)

void unit_test_int_vec() {

    {
        test_section("int_vec_init / int_vec_free");

        int_vec vec;

        int_vec_init(&vec);
        test_assert(vec.capacity == 0);
        test_assert(vec.length == 0);
        test_assert(vec.data == NULL);

        int_vec_reserve(&vec, 20);
        test_assert(vec.capacity == 20);
        test_assert(vec.length == 0);
        test_assert(vec.data != NULL);

        int_vec_resize(&vec, 30);
        test_assert(vec.capacity >= 30);
        test_assert(vec.length == 30);
        test_assert(vec.data != NULL);

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_truncate / int_vec_clear");

        int_vec vec;

        int_vec_init(&vec);
        test_assert(vec.capacity == 0);
        test_assert(vec.length == 0);
        test_assert(vec.data == NULL);

        int_vec_resize(&vec, 20);
        test_assert(vec.capacity >= 20);
        test_assert(vec.length == 20);
        test_assert(vec.data != NULL);

        int_vec_truncate(&vec, 10);
        test_assert(vec.capacity >= 20);
        test_assert(vec.length == 10);
        test_assert(vec.data != NULL);

        int_vec_clear(&vec);
        test_assert(vec.capacity >= 20);
        test_assert(vec.length == 0);
        test_assert(vec.data != NULL);

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_shrink_to_fit");

        int_vec vec;
        int_vec_init(&vec);

        for (int i = 0; i < 1000000; i++) {
            int_vec_push(&vec, i);
        }

        for (int i = 0; i < 999999; i++) {
            int_vec_pop(&vec, NULL);
        }

        printf("Length:   %zu\n", vec.length);
        printf("Capacity: %zu\n", vec.capacity);

        int_vec_shrink_to_fit(&vec);

        printf("Length:   %zu\n", vec.length);
        printf("Capacity: %zu\n", vec.capacity);

    }

    {
        test_section("vec_push");
     
        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 1000; i++) {
            int_vec_push(&vec, (int) i * 2);
        }

        test_assert(vec.data[1] == 2);
        test_assert(vec.data[999] == 999 * 2);
        test_assert(int_vec_push(&vec, 10) == 0);

        int_vec_free(&vec);
    }

    {
        test_section("vec_pop");
        int_vec vec;

        int_vec_init(&vec);

        int_vec_push(&vec, 123);
        int_vec_push(&vec, 456);
        int_vec_push(&vec, 789);

        int value;
        int_vec_pop(&vec, &value);
        test_assert(value == 789);
        int_vec_pop(&vec, &value);
        test_assert(value == 456);
        int_vec_pop(&vec, &value);
        test_assert(value == 123);

        int_vec_free(&vec);
    }

    {
        test_section("vec_insert");

        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 1000; i++) {
            int_vec_push(&vec, (int) i);
        }

        test_assert(vec.data[0] == 0);
        test_assert(vec.data[vec.length - 1] == 999);
        int_vec_insert(&vec, 10, 123);
        test_assert(vec.data[10] == 123);
        test_assert(vec.length == 1001);
        int_vec_insert(&vec, vec.length - 2, 678);
        test_assert(vec.data[999] == 678);
        test_assert(int_vec_insert(&vec, 10, 123) == 0);
        int_vec_insert(&vec, vec.length, 789);
        test_assert(vec.data[vec.length - 1] == 789);

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_remove");
        int_vec vec;

        int_vec_init(&vec);

        int_vec_push(&vec, 123);
        int_vec_push(&vec, 456);
        int_vec_push(&vec, 789);

        test_assert(vec.length == 3);
        test_assert(vec.data[0] == 123);
        test_assert(vec.data[1] == 456);
        test_assert(vec.data[2] == 789);

        int_vec_remove(&vec, 1);
        test_assert(vec.length == 2);
        test_assert(vec.data[0] == 123);
        test_assert(vec.data[1] == 789);

        int_vec_remove(&vec, 0);
        test_assert(vec.length == 1);
        test_assert(vec.data[0] == 789);

        int_vec_remove(&vec, 0);
        test_assert(vec.length == 0);

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_get");

        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 100; i++) {
            int_vec_push(&vec, (int) i * 2);
        }

        for (size_t i = 0; i < 10; i++) {
            test_assert(*int_vec_get(&vec, i) == i * 2);
        }

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_set");

        int_vec vec;
        int_vec_init(&vec);

        int_vec_resize(&vec, 10);

        for (size_t i = 0; i < 10; i++) {
            int_vec_set(&vec, i, (int) i * 2);
        }

        for (size_t i = 0; i < 10; i++) {
            const int* value_ptr = int_vec_get(&vec, i);
            test_assert(*value_ptr == i * 2);
        }

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_set");

        int_vec vec;
        int_vec_init(&vec);

        int_vec_push(&vec, 10);
        int_vec_push(&vec, 20);
        int_vec_push(&vec, 30);

        if (int_vec_set(&vec, 1, 999) != 0) {
            printf("Index out of bounds\n");
        }

        test_assert(vec.data[1] == 999);
        const int* value_ptr = int_vec_get(&vec, 1);
        test_assert(*value_ptr == 999);

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_front / int_vec_back / int_vec_find");

        int_vec vec;
        int_vec_init(&vec);

        int_vec_push(&vec, 10);
        int_vec_push(&vec, 20);
        int_vec_push(&vec, 30);

        int value;
        int result = int_vec_front(&vec, &value);
        test_assert(value == 10);
        test_assert(result == 0);

        result = int_vec_back(&vec, &value);
        test_assert(value == 30);
        test_assert(result == 0);

        size_t index;
        result = int_vec_find(&vec, 20, &index);
        test_assert(index == 1);
        test_assert(result == 0);

        result = int_vec_find(&vec, 99, &index);
        test_assert(result == -1);

        int_vec_free(&vec);
    }

    {
        test_section("vec_truncate");
        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 1000; i++) {
            int_vec_push(&vec, 0);
        }
        
        int_vec_truncate(&vec, 10000);
        test_assert(vec.length == 1000);
        int_vec_truncate(&vec, 900);
        test_assert(vec.length == 900);
        int_vec_free(&vec);
    }

    {
        test_section("int_vec_next");

        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 10; i++) {
            int_vec_push(&vec, (int) i * 10);
        }

        size_t i = 0;
        const int* value;

        while ((value = int_vec_next(&vec, &i)) != NULL) {
            // printf("%zu: %d\n", i, *value);
            test_assert(*value == ((i - 1) * 10));
        }

        int_vec_free(&vec);
    }

    {
        test_section("int_vec_prev");

        int_vec vec;
        int_vec_init(&vec);

        for (size_t i = 0; i < 10; i++) {
            int_vec_push(&vec, (int)i * 10);
        }

        size_t i = vec.length;
        const int* value;

        while ((value = int_vec_prev(&vec, &i)) != NULL) {
            // printf("%zu: %d\n", i, *value);
            test_assert(*value == (i * 10));
        }

        int_vec_free(&vec);
    }

    test_print_res();
}

// helper functions
static void test_section(const char* desc)
{
    printf("--- %s\n", desc);
}

static void test_assert_func(int cond, const char* cond_str, const char* file, int line)
{
    printf("[%s] %s:%d: %s\n", cond ? "PASS" : "FAIL", file, line, cond_str);

    if (cond) {
        pass_count++;
    }
    else {
        fail_count++;
    }
}

// Note: Stringification of '#cond':
// Stringification functions exclusively within the preprocessor (macro)
// to convert the code text into a string
#define test_assert(cond) test_assert_func((cond), #cond, __FILE__, __LINE__)

static void test_print_res()
{
    printf("------------------------------------------------------------\n");
    printf("-- Results:   %3d Total    %3d Passed    %3d Failed       --\n", pass_count + fail_count, pass_count, fail_count);
    printf("------------------------------------------------------------\n");
}

// ===========================================================================
// End-of-File
// ===========================================================================
