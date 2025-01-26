// ===========================================================================
// MemoryManagement.cpp
// ===========================================================================

#include <iostream>
#include <print>
//#include <string>
//#include <unordered_map>

#include  <stdint.h>

namespace MemoryManagement {

    // =======================================================================
    // Examine Stack bevaviour

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

    static void test_examine_stack_bevaviour() {

        auto i{ 0 };
        std::cout << "main():  " << std::addressof(i) << " - i: " << i << '\n';
        func2();
        func1();
    }

    // =======================================================================
    // Examine Stack Size

    static void func (const std::byte* stack_bottom_addr) {
        std::byte data[1024] = {};
        std::cout << stack_bottom_addr - data << '\n';

//#pragma warning(push)
//#pragma warning(disable : 4717)

        func(stack_bottom_addr);

//#pragma warning(pop)

    }

    static void test_examine_stack_size() {

        std::byte b;
        func(&b);
    }
}

// =================================================================

void memory_management()
{
    using namespace MemoryManagement;

    //test_examine_stack_bevaviour();
    test_examine_stack_size();
}

// ===========================================================================
// End-of-File
// ===========================================================================
