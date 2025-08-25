// ===========================================================================
// ObjectPool_ThreadSafe_Test.cpp // Performance Optimization Advanced
// ===========================================================================

// #include "../LoggerUtility/ScopedTimer.h"
//#include "../Person/Person.h"

#include "ObjectPool_ThreadSafe.h"

#include <memory>
#include <print>

namespace ObjectPool_ThreadSafe_SimpleTest {

    static void test_object_pool_thread_safe_01()
    {
        using namespace FixedSizeObjectPoolThreadSafe;

        ObjectPool<int> pool;

        auto ptr = pool.construct(123);

        std::println("*ptr: {}", *ptr);

        pool.destroy(ptr);
        std::println("Done.");
    }
}

void test_object_pool_thread_safe()
{
    using namespace ObjectPool_ThreadSafe_SimpleTest;

    test_object_pool_thread_safe_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
