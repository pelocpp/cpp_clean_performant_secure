// ===========================================================================
// CustomAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../Person/Person.h"

#include "CustomAllocator.h"

#include <vector>

namespace Custom_Allocator_Test {

    static void test_custom_allocator_01()
    {
        CustomAllocator<int> alloc;

        std::vector<int, CustomAllocator<int>> vec(alloc);

        // std::vector<int, CustomAllocator<int>> vec;  // works too

        // vec.reserve(3);  // remove comment, see output

        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);

        for (int i : vec) {
            std::println("{} ", i);
        }
    }

    static void test_custom_allocator_02()
    {
        std::vector<Person, CustomAllocator<Person>> vec;

        vec.push_back({ "Sepp", "Mueller", 30 });
        vec.push_back({ "Hans", "Wagner", 30 });
        vec.push_back({ "Anton", "Meier", 30 });

        for (const auto& person : vec) {
            std::println("{} ", person);
        }
    }

    static void test_custom_allocator_03()
    {
        CustomAllocator<int> alloc;

        std::vector<int, CustomAllocator<int>> vec(alloc);

        // std::vector<int, SimpleAllocator<int>> vec;  // works too

        std::println("Adding elements to vector:");
        vec.push_back(1);
        vec.push_back(2);
        vec.push_back(3);

        std::println("Vector contents: ");
        for (int i : vec) {
            std::print("{} ", i);
        }
        std::println("");

        std::println("Resizing vector to 1 element:");
        vec.resize(1);

        std::println("Clearing vector:");
        vec.clear();
    }
}

void test_custom_allocator()
{
    using namespace Custom_Allocator_Test;

    test_custom_allocator_01();
    test_custom_allocator_02();
    test_custom_allocator_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
