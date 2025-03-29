// ===========================================================================
// SecureProgrammingMoreIssues.cpp // Secure Programming
// More Issues 
// ===========================================================================

// don't use the secure versions of the CRT library functions
#define _CRT_SECURE_NO_WARNINGS 

#include <atomic>
#include <cstring>
#include <iostream>
#include <print>
#include <thread>
#include <vector>

namespace SecureProgrammingMoreIssues {

    namespace UsingPointers {

        static void decay(const int* ages) {
            // Size of the pointer = 8
            std::println("Size of an 'int*' pointer:          {}", sizeof(ages));

            // Compile Error
            // std::cout << std::size(ages) << '\n';
        }

        static void test_using_pointers_demonstrating_decay() {

            int ages[] = { 15, 30, 50 };
            // Number of elements = 3
            std::println("Number of array elements:           {}", std::size(ages));

            // Size of an element = 4
            std::println("Size of a single array element:     {}", sizeof(ages[0]));

            // Size of array = 12 (= 3 * 4)
            std::println("Number of bytes used by this array: {}", sizeof(ages));
            decay(ages);
        }

        // ===============================================================================

        struct Numbers
        {
            int m_numbers[1000];
        };

        static void funcA(int* ptr) {
            ptr[0] = 123;                    // watch this array entry in the calling function
        }

        static void funcB(struct Numbers numbers) {

            size_t s = sizeof(numbers);
            s = sizeof(struct Numbers);
            numbers.m_numbers[0] = 123;      // watch this array entry in the calling function
        }

        static void funcC(struct Numbers* pNumbers) {

            size_t s = sizeof(pNumbers);
            s = sizeof(struct Numbers);
            (*pNumbers).m_numbers[0] = 123;  // watch this array entry in the calling function
            // oder etwas geschmeidiger
            pNumbers->m_numbers[0] = 123;
        }

        static void test_calling_conventions()
        {
            int numbers[1000] = {};
            funcA(numbers);                  // passing an array

            struct Numbers num = {};
            funcB(num);                      // passing a structure variable

            funcC(&num);                     // passing a structure variable
        }

        // ===============================================================================

        static void test_using_pointers_std_size() {

            std::vector<int> numbers{ 1, 2, 3 };
            std::println("Number of std::vector elements:           {}", std::size(numbers));

            numbers.push_back(4);
            numbers.push_back(5);
            numbers.push_back(6);

            std::println("Number of std::vector elements:           {}", std::size(numbers));
        }

        static void test_using_pointers() {

            test_using_pointers_demonstrating_decay();
            test_calling_conventions();
            test_using_pointers_std_size();
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

    namespace MemoryLeaks {

        static void test_memory_leaks()
        {
            int* ptr = new int[10]; // memory allocated but never deallocated
        }
    }

    namespace RaceConditions {

        const int MaxCount = 100'000;

        long counter{};
        std::atomic<long> atomicCounter{};

        static void increment() {

            for (int i = 0; i != MaxCount; ++i) {
                ++counter;
            }
        }

        static void incrementAtomic() {

            for (int i = 0; i != MaxCount; ++i) {
                ++atomicCounter;
            }
        }

        static void test_race_conditions_unsafe()
        {
            std::println("Counter: {}", counter);

            std::thread t1{ increment };
            std::thread t2{ increment };

            t1.join();
            t2.join();

            std::println("Counter: {}", counter); // expected 200000, but result is non-deterministic
        }

        static void test_race_conditions_safe()
        {
            std::println("Counter: {}", atomicCounter.load());

            std::thread t1{ incrementAtomic };
            std::thread t2{ incrementAtomic };

            t1.join();
            t2.join();

            std::println("Counter: {}", atomicCounter.load());
        }

        static void test_race_conditions()
        {
            test_race_conditions_unsafe();
            test_race_conditions_safe();
        }
    }
}

// =================================================================

void secure_programming_more_issues()
{
    using namespace SecureProgrammingMoreIssues;

    UsingPointers::test_using_pointers();
    DanglingReferences::test_dangling_reference();
    MemsetIssue::test_disappearing_memset();
    MemoryLeaks::test_memory_leaks();
    RaceConditions::test_race_conditions();
}

// ===========================================================================
// End-of-File
// ===========================================================================
