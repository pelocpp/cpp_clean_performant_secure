#include "IntVec.h"

#include <stdio.h>
#include <string.h>

#include <assert.h>   // evtl. mit test_assert ersetzen

//#define test_section(desc)\
//  do {\
//    printf("--- %s\n", desc);\
//  } while (0)

static void test_section(const char* desc)
{
    printf("--- %s\n", desc);
}

//#define test_assert(cond)\
//  do {\
//    int pass__ = cond;\
//    printf("[%s] %s:%d: ", pass__ ? "PASS" : "FAIL", __FILE__, __LINE__);\
//    printf((strlen(#cond) > 50 ? "%.47s...\n" : "%s\n"), #cond);\
//    if (pass__) { pass_count++; } else { fail_count++; }\
//  } while (0)

//void test_assert()
//{
//        int pass__ = cond;
//        printf("[%s] %s:%d: ", pass__ ? "PASS" : "FAIL", __FILE__, __LINE__);
//        printf((strlen(#cond) > 50 ? "%.47s...\n" : "%s\n"), #cond);\
//        if (pass__) { pass_count++; } else { fail_count++; }\
//}



void unit_test_int_vec(void) {

    {
        test_section("vec_push");
        int_vec v;
        int_vec_init(&v);
        int i;
        for (i = 0; i < 1000; i++) {
            int_vec_push(&v, i * 2);
        }

        //test_assert(v.data[1] == 2);
        //test_assert(v.data[999] == 999 * 2);
        //test_assert(vec_push(&v, 10) == 0);

        assert(v.data[1] == 2);
        assert(v.data[999] == 999 * 2);
        assert(int_vec_push(&v, 10) == 0);

        int_vec_deinit(&v);
    }
}