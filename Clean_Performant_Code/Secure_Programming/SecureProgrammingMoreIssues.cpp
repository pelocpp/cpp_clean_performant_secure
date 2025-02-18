// ===========================================================================
// SecureProgrammingMoreIssues.cpp
// More Issues 
// ===========================================================================

// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 

#include <complex>
#include <cstring>
#include <iostream>
#include <print>

namespace SecureProgrammingMoreIssues {

    namespace InfiniteLoop {

        std::complex<float> delta;
        std::complex<float> mc[4];

        // sanitizer beispiel !!!!!

        static void test_infinite_loop() {

            for (int di = 0; di < 4; di++, delta = mc[di]) {

                std::println("{}", di);
            }
        }
    }

    namespace UsingPointers {

        static void decay(const int* ages) {
            // Size of the pointer = 8
            std::println("Size of an 'int*' pointer:          {}", sizeof(ages));

            // Compile Error
            // std::cout << std::size(ages) << '\n';
        }

        static void test_using_pointers() {

            int ages[] = { 15, 30, 50 };
            // Number of elements = 3
            std::println("Number of array elements:           {}", std::size(ages));

            // Size of an element = 4
            std::println("Size of a single array element:     {}", sizeof(ages[0]));

            // Size of array = 12 (= 3 * 4)
            std::println("Number of bytes used by this array: {}", sizeof(ages));
            decay(ages);
        }
    }
}

// =================================================================

void secure_programming_more_issues()
{
    using namespace SecureProgrammingMoreIssues;

    InfiniteLoop::test_infinite_loop();
    UsingPointers::test_using_pointers();
}

// ===========================================================================
// End-of-File
// ===========================================================================
