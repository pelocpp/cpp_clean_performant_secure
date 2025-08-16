// ===========================================================================
// StandardAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../Person/Person.h"

#include <memory>

namespace Standard_Allocator_Test {

    static void test_standard_allocator_01()
    {
        // create an allocator to use
        std::allocator<Person> alloc;

        // allocate an uninitialized memory block for 1 instance of 'Person'
        auto* memory{ alloc.allocate(1) };

        // use 'placement new' operator to construct a 'Person' object in place
        ::new (memory) Person{};

        // or using std::construct_at
        // std::construct_at(memory, Person{ "Hans", "Mueller", 30 });

        // destroy 'Person' instance
        std::destroy_at(memory);

        // deallocate memory block
        alloc.deallocate(memory, 1);

        memory = nullptr;
    }
}

void test_standard_allocator()
{
    using namespace Standard_Allocator_Test;

    test_standard_allocator_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
