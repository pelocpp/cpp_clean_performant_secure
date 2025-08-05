// ===========================================================================
// CustomAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../Person/Person.h"

#include "CustomAllocator.h"

#include <vector>

static void test_custom_allocator_01()
{
    // create an allocator to use
    std::allocator<Person> alloc;

    // allocate an uninitialized memory block for 1 instance of 'Person'
    auto* memory{ alloc.allocate(1) };

    // use 'placement new' operator to construct a 'Person' object in place
    ::new (memory) Person{};

    // destroy 'Person' instance
    std::destroy_at(memory);

    // deallocate memory block
    alloc.deallocate(memory, 1);

    memory = nullptr;
}

static void test_custom_allocator_02()
{
    SimpleAllocator<int> alloc;

    std::vector<int, SimpleAllocator<int>> vec(alloc);

    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (int i : vec) {
        std::println("{} ", i);
    }
    std::println();
}

static void test_custom_allocator_03()
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
    test_custom_allocator_01();
    test_custom_allocator_02();
    test_custom_allocator_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
