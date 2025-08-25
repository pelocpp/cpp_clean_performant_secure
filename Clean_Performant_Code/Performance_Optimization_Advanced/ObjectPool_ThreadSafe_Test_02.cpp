// ===========================================================================
// ObjectPool_ThreadSafe_Test_02.cpp // Performance Optimization Advanced
// From: https://radiantsoftware.hashnode.dev/c-lock-free-object-pool
// Status: Not yet prepared // To be Done
// ===========================================================================

// #include "../LoggerUtility/ScopedTimer.h"
#include "../Person/Person.h"

#include "ObjectPool_ThreadSafe_02.h"

#include <memory>
#include <print>

namespace ObjectPool_ThreadSafe_SimpleTest {

    static void test_object_pool_thread_safe_01()
    {
        using PersonPool = Pool<Person>;

        // default allocator for Persons
        std::allocator<Person> alloc;

        PersonPool pool;

        pool.allocate(alloc, 20);

        //template <typename AllocatorType>
        //void allocate(AllocatorType & aAllocator, int aCapacity);

        //auto person{ pool.allocate("Sepp", "Mueller", (size_t)30) };

        //std::println("{} {} has age {}",
        //    person->getFirstname(), person->getLastname(), person->getAge());

        std::println("Done.");
    }
}

void test_object_pool_thread_safe_02()
{
    using namespace ObjectPool_ThreadSafe_SimpleTest;

    test_object_pool_thread_safe_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
