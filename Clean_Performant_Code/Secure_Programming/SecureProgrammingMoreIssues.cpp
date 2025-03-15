// ===========================================================================
// SecureProgrammingMoreIssues.cpp
// More Issues 
// ===========================================================================

// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 

#include <cstring>
#include <iostream>
#include <print>
#include <vector>

namespace SecureProgrammingMoreIssues {

    namespace UsingPointers {

        static void decay(const int* ages) {
            // Size of the pointer = 8
            std::println("Size of an 'int*' pointer:          {}", sizeof(ages));

            // Compile Error
            // std::cout << std::size(ages) << '\n';
        }

        static void test_using_pointers_demstrating_decay() {

            int ages[] = { 15, 30, 50 };
            // Number of elements = 3
            std::println("Number of array elements:           {}", std::size(ages));

            // Size of an element = 4
            std::println("Size of a single array element:     {}", sizeof(ages[0]));

            // Size of array = 12 (= 3 * 4)
            std::println("Number of bytes used by this array: {}", sizeof(ages));
            decay(ages);
        }

        static void test_using_pointers_std_size() {

            std::vector<int> numbers{ 1, 2, 3 };
            std::println("Number of std::vector elements:           {}", std::size(numbers));

            numbers.push_back(4);
            numbers.push_back(5);
            numbers.push_back(6);

            std::println("Number of std::vector elements:           {}", std::size(numbers));
        }

        static void test_using_pointers() {

            test_using_pointers_demstrating_decay();
            test_using_pointers_std_size();
        }
    }

    namespace MemsetIssue {

        // https://cwe.mitre.org/data/definitions/14.html

        // Compiler Removal of Code to Clear Buffers

        static bool connectToServer(char* pwd) {
            std::println("{}", pwd);
            return true;
        }

        static bool getPasswordFromUser(char* pwd, size_t pwdSize) {
            strncpy(pwd, "My super secret password", pwdSize);
            std::println("{}", pwd);
            return true;
        }

        static void test_disappearing_memset()
        {
            char pwd[64];

            if (getPasswordFromUser(pwd, sizeof(pwd))) {
                if (connectToServer(pwd)) {
                    // interaction with server
                }
            }

            std::memset(pwd, 0, sizeof(pwd)); // <- Removed by the optimizer !!!
        }
    }

    namespace DanglingReferences {

        struct Data {
            Data(int& value) : m_value(value) {}
            int& m_value;
        };

        static Data function() {

            int value = 123;

            Data data(value);

            std::println("value: {}", value);

            return data;                         // implicitly returning reference to local value
        }

        static void test_dangling_reference()
        {
            Data data = function();
            std::println("{}", data.m_value);    // Oooooops
        }
    }
}

// =================================================================

void secure_programming_more_issues()
{
    using namespace SecureProgrammingMoreIssues;

    UsingPointers::test_using_pointers();
    MemsetIssue::test_disappearing_memset();
    DanglingReferences::test_dangling_reference();
}

// ===========================================================================
// End-of-File
// ===========================================================================
