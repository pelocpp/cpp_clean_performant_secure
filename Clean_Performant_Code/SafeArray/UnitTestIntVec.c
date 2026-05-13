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

#include <stdio.h>
#include <string.h>

// Globale Variablen (müssen im Code definiert sein)
static int pass_count = 0;
static int fail_count = 0;

static void test_assert_func(int cond, const char* cond_str, const char* file, int line)
{
    printf("[%s] %s:%d: ", cond ? "PASS" : "FAIL", file, line);

    // Kürzung der Bedingung auf maximal 50 Zeichen bei der Ausgabe
    printf((strlen(cond_str) > 50 ? "%.47s...\n" : "%s\n"), cond_str);

    if (cond) {
        pass_count++;
    }
    else {
        fail_count++;
    }
}

// Note: '#cond':  Stringification functions exclusively within the preprocessor (macro)
// to convert the code text into a string
#define test_assert(cond) test_assert_func((cond), #cond, __FILE__, __LINE__)

void unit_test_int_vec(void) {

    {
        test_section("vec_push");
     
        int_vec v;
        int_vec_init(&v);
        
        int i;
        for (i = 0; i < 1000; i++) {
            int_vec_push(&v, i * 2);
        }

        test_assert(v.data[1] == 2);
        test_assert(v.data[999] == 999 * 2);
        test_assert(int_vec_push(&v, 10) == 0);

        //assert(v.data[1] == 2);
        //assert(v.data[999] == 999 * 2);
        //assert(int_vec_push(&v, 10) == 0);

        int_vec_deinit(&v);
    }

    {
        test_section("vec_pop");
        int_vec v;

        int_vec_init(&v);

        int_vec_push(&v, 123);
        int_vec_push(&v, 456);
        int_vec_push(&v, 789);

        test_assert(int_vec_pop(&v) == 789);
        test_assert(int_vec_pop(&v) == 456);
        test_assert(int_vec_pop(&v) == 123);

        int_vec_deinit(&v);
    }
}