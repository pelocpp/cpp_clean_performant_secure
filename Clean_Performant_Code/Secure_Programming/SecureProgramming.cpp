// ===========================================================================
// SecureProgramming.cpp
// ===========================================================================

// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 

#include <complex>
#include <cstring>
#include <iostream>
#include <print>

namespace SecureProgrammingPractices {

    namespace BufferOverflowStack {

        static void corrupt_stack(const char* input) {

            char buffer[32];
            strcpy(buffer, input);
            std::println("{}", buffer);
        }

        static void test_corrupt_stack() {

            corrupt_stack("The quick brown fox jumps over the lazy dog");
        }

        static void corrupt_stack_02(const char* input) {

            char buffer[32];
            std::copy(input, input + 32, buffer);
            std::println("{}", buffer);
        }

        static void test_corrupt_stack_02() {

            corrupt_stack_02("The quick brown fox jumps over the lazy dog");
        }
    }

    namespace BufferOverflowHeap {

        static void corrupt_heap(const char* input) {

            char* buffer = new char[32];

            strcpy(buffer, input);
            std::println("{}", buffer);

            delete[] buffer;
        }

        static void test_corrupt_heap() {

            corrupt_heap("The quick brown fox jumps over the lazy dog");
        }
    }

    namespace TypePunning {

        static void test_type_punning() {

            uint16_t data { 0x1234 };

            uint16_t* u16 = &data;

            uint32_t* u32 = (uint32_t*) &data;

            *u32 = 0x12345678; // de-referencing invokes undefined behavior
        }
    }

    static int x[100];

    namespace UsingAddressSanitizer {

        static void test_address_sanitizer() {

            std::println("Hello!");
            x[100] = 5; // Boom!
            std::println("Boom!");
        }
    }

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
}

// =================================================================

void secure_programming_practices()
{
    using namespace SecureProgrammingPractices;

    //BufferOverflowStack::test_corrupt_stack();
    //BufferOverflowStack::test_corrupt_stack_02();
    //BufferOverflowHeap::test_corrupt_heap();
    //TypePunning::test_type_punning();

   // UsingAddressSanitizer::test_address_sanitizer();

    InfiniteLoop::test_infinite_loop();

    std::println("Done.");
}

// ===========================================================================
// End-of-File
// ===========================================================================
