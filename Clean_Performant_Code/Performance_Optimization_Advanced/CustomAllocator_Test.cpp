// ===========================================================================
// CustomAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "CustomAllocator.h"

#include <vector>

static void test_custom_allocator_01()
{
    SimpleAllocator<int> alloc;

    std::vector<int, SimpleAllocator<int>> vec(alloc);

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (int i : vec) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

static void test_custom_allocator_02()
{
    SimpleAllocator<int> alloc;

    std::vector<int, SimpleAllocator<int>> vec(alloc);

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

void test_custom_allocator()
{
 //   test_custom_allocator_01();
    test_custom_allocator_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
