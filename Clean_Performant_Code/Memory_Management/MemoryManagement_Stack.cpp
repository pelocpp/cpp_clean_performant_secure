// ===========================================================================
// MemoryManagement_Stack.cpp // Memory Management
// ===========================================================================

#include <cstddef>
#include <iostream>
#include <memory>
#include <print>
#include <string>

namespace MemoryManagement {

    namespace Stack_Debug_Versus_Release_Mode {

        // =============================================================== 
        // Examine Stack Layout: Debug versus Release Mode

        static void test_stack_debug_versus_release_mode()
        {
            int a = 1;
            int b = 2;
            int c = 3;
            int d = 4;

            int* ap = &a;
            int* bp = &b;
            int* cp = &c;
            int* dp = &d;

            unsigned long long d1 = (unsigned long long)bp - (unsigned long long)ap;
            unsigned long long d2 = (unsigned long long)cp - (unsigned long long)bp;
            unsigned long long d3 = (unsigned long long)dp - (unsigned long long)cp;

            std::cout << "&a:  " << &a << std::endl;
            std::cout << "&b:  " << &b << std::endl;
            std::cout << "&c:  " << &c << std::endl;
            std::cout << "&d:  " << &d << std::endl;

            std::println("Diff a <=> b: {}", d1);
            std::println("Diff b <=> c: {}", d2);
            std::println("Diff c <=> d: {}", d3);
        }
    }

    namespace Stack_Behaviour {

        // =======================================================================
        // Examine Stack behaviour

        static void func1()
        {
            auto i{ 1 };
            std::cout << "func1(): " << std::addressof(i) << " - i: " << i << '\n';
        }

        static void func2()
        {
            auto i{ 2 };
            std::cout << "func2(): " << std::addressof(i) << " - i: " << i << '\n';
            func1();
        }

        static void test_examine_stack_behaviour() {

            auto i{ 0 };
            std::cout << "main():  " << std::addressof(i) << " - i: " << i << '\n';
            func2();
            func1();
        }
    }

    namespace Stack_Size {

        // =======================================================================
        // Examine Stack Size

        static void func(const std::byte* stack_bottom_addr) {
            std::byte data[1024] = {};
            std::cout << stack_bottom_addr - data << '\n';

            func(stack_bottom_addr);
        }

        static void test_examine_stack_size() {

            std::byte b{};
            func(&b);
        }
    }
}

void memory_management_stack()
{
    using namespace MemoryManagement;

    Stack_Debug_Versus_Release_Mode::test_stack_debug_versus_release_mode();

    Stack_Behaviour::test_examine_stack_behaviour();
    //Stack_Size::test_examine_stack_size();               // crashes (intentionally)
}

// ===========================================================================
// End-of-File
// ===========================================================================
