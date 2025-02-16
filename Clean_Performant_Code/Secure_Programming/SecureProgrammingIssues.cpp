// ===========================================================================
// SecureProgrammingIssues.cpp // Secure Programming
// Exploitability // Vulnerability
// ===========================================================================

// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 

#include <complex>
#include <cstring>
#include <iostream>
#include <print>
#include <cstdio>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

namespace SecureProgrammingExploitability {

    namespace UnsignedIntegerWraparound {
    
        // https://cwe.mitre.org/data/definitions/190.html

        // Unsigned Integer Wraparound
        // Defined behaviour: Wrapping

        static const unsigned short first_len = SHRT_MAX;
        static const unsigned short second_len = 256;
        static const unsigned short buf_len = 256;

        static char first[first_len];
        static char second[second_len];
        static char buf[buf_len];

        static void test_unsigned_integer_wraparound() {

            unsigned short len = first_len + second_len;  // < -sum == 255

            if (len <= 256) {                             // <- sum == 255
                memcpy(buf, first, first_len);
            }
        }
    }

    namespace SignedIntegerOverflow {

        // https://cwe.mitre.org/data/definitions/190.html

        // Signed Integer Overflow
        // Undefined behaviour: Overflow

        static const signed short first_len = SHRT_MAX;
        static const signed short second_len = 256;
        static const signed short buf_len = 256;

        static char first[first_len];
        static char second[second_len];
        static char buf[buf_len];

        static void test_signed_integer_overflow() {

            signed short len = first_len + second_len;  // <- UB: negative

            if (len <= 256) {
                memcpy(buf, first, first_len);
            }
        }
    }

    namespace NumericTruncationError {

        // https://cwe.mitre.org/data/definitions/197.html

        // Numeric Truncation Error
        // Implementation Defined Behaviour (IDB):
        // Integer Conversions do result in lost or misinterpreted data

        static const unsigned short first_len = USHRT_MAX - 256;
        static const unsigned short second_len = 256;
        static const unsigned short buf_len = 256;

        static char first[first_len];
        static char second[second_len];
        static char buf[buf_len];

        static void test_numeric_Truncation_error() {

            signed short len = first_len + second_len;  // <- IDB (negative) // unsigned sum is assigned to signed variable

            if (len <= 256) {
                memcpy(buf, first, first_len);
            }
        }
    }

    namespace StackBasedBufferOverflow {

        // https://cwe.mitre.org/data/definitions/121.html
        // Stack-based Buffer Overflow
        
        // https://cwe.mitre.org/data/definitions/242.html
        // Use of Inherently Dangerous Function

        // https://cwe.mitre.org/data/definitions/787.html
        // Out-of-bounds Write

        static void test_stack_based_buffer_overflow() {

            //char buffer[10];
            //gets(buffer); // <- Write outside  // Deprecated in C++ 11 // Removed in C++ 14
        }
    }

    namespace HeapBasedBufferOverflow {

        // https://cwe.mitre.org/data/definitions/122.html
        // Heap-based Buffer Overflow

        static void test_heap_based_buffer_overflow_internal(const char* data) {

            char* buf = (char*) malloc(sizeof(char) * 10);

            // Heap Buffer Overflow when data is bigger than 10
            strcpy(buf, data);     // <- Write outside 
            free(buf);
        }

        static void test_heap_based_buffer_overflow() {

            test_heap_based_buffer_overflow_internal("Very, very long string");
        }
    }

    namespace BufferUnderwriteUnderflow {

        // https://cwe.mitre.org/data/definitions/124.html
        // Buffer Underwrite Underflow

        static void test_buffer_underwrite_underflow () {

            char src[12];

            strncpy(src, "Hello World", sizeof(src));

            size_t length = strlen(src);

            int index = (length - 1);
            while (src[index] != ':') {
                src[index] = '\0';
                index--;
            }
        }
    }

    namespace UseAfterFree {

        // https://cwe.mitre.org/data/definitions/416.html
        // Use After Free
        // Don't access freed memory

        static void test_use_after_free() {

            char* buffer = new char[256];
            memset(buffer, '!', 256);

            bool error = true;
            if (error)
                delete[] buffer;

            if (error) {
                // Use after free when error is true
                // printf("%lu\n", strlen(buffer));

                size_t len = strlen(buffer);   // use after free
            }
        }
    }

    namespace DoubleFree {

        // https://cwe.mitre.org/data/definitions/415.html
        // Double Free
        // Properly deallocate dynamically allocated resources

        static void test_double_free() {

            char* buffer = new char[256];
            memset(buffer, '!', 256);

            bool error = true;
            if (error)
                delete[] buffer;

            // ....

            delete[] buffer; // second free
        }
    }

    namespace IncorrectTypeConversion {

        // https://cwe.mitre.org/data/definitions/704.html
        // Use of Externally-Controlled Format String
        // Note: This is a feature of the language :)
        // Do not cast to an unrelated type

        static void test_incorrect_type_conversion() {

            struct A {};
            struct B {};

            struct A* a = (struct A*) malloc(sizeof(struct A));

            // cast to unrelated type
            struct B* b = (struct B*) a;
        }
    }

    namespace UseOfExternalFormatString {

        // https://cwe.mitre.org/data/definitions/134.html
        // Use of Externally-Controlled Format String
        // Only use valid format strings

        static void test_use_of_external_format_string_internal(const char* format, const char* str) {

            printf(format, str);
        }

        static void test_use_of_external_format_string() {

        //    test_use_of_external_format_string_internal("%s", "Very, very long string");

            test_use_of_external_format_string_internal("%s %d", "Very, very long string");
        }
    }
}

// =================================================================

void secure_programming_exploitability()
{
    using namespace SecureProgrammingExploitability;

    UnsignedIntegerWraparound::test_unsigned_integer_wraparound();
    SignedIntegerOverflow::test_signed_integer_overflow();
    NumericTruncationError::test_numeric_Truncation_error();
    StackBasedBufferOverflow::test_stack_based_buffer_overflow();
    HeapBasedBufferOverflow::test_heap_based_buffer_overflow();
    BufferUnderwriteUnderflow::test_buffer_underwrite_underflow();
    UseAfterFree::test_use_after_free();
    DoubleFree::test_double_free();
    IncorrectTypeConversion::test_incorrect_type_conversion();
    UseOfExternalFormatString::test_use_of_external_format_string();
}

// ===========================================================================
// End-of-File
// ===========================================================================
