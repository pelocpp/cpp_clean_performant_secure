// ===========================================================================
// ObjectPool_FixedSize_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "../LoggerUtility/ScopedTimer.h"
#include "../Person/Person.h"

#include "ObjectPool_FixedSize.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <print>
#include <type_traits>

namespace ObjectPool_FixedSize_SimpleTest {

    using namespace FixedSizeObjectPool;

    static void test_object_pool_01()
    {
        ObjectPool<int> pool;

        auto ptr = pool.construct(123);

        std::println("*ptr:           {}", *ptr);

        pool.destroy(ptr);
    }

    static void test_object_pool_02()
    {
        ObjectPool<Person> pool;

        auto ptr = pool.construct(Person{ "Hans", "Mueller", 30 });
        std::println("Person: {}", *ptr);

        pool.destroy(ptr);
    }

    static void test_object_pool_03()
    {
        ObjectPool<int> pool;

        auto ptr1 = pool.construct(123);
        auto ptr2 = pool.construct(456);
        auto ptr3 = pool.construct(789);

        std::println("*ptr: {}", *ptr1);
        std::println("*ptr: {}", *ptr2);
        std::println("*ptr: {}", *ptr3);

        pool.destroy(ptr1);
        pool.destroy(ptr2);
        pool.destroy(ptr3);
    }

    static void test_object_pool_04()
    {
        ObjectPool<Person> pool;

        auto ptr1 = pool.construct(Person{ "Hans", "Mueller", 30 });
        auto ptr2 = pool.construct(Person{ "Susi", "Wagner", 30 });
        auto ptr3 = pool.construct(Person{ "Gerd", "Meier", 30 });

        std::println("Person: {}", *ptr1);
        std::println("Person: {}", *ptr2);
        std::println("Person: {}", *ptr3);

        pool.destroy(ptr1);
        pool.destroy(ptr2);
        pool.destroy(ptr3);
    }
}

namespace ObjectPool_FixedSize_AdvancedTest {

    using namespace FixedSizeObjectPool;

    static void test_object_pool_10()
    {
        ObjectPool<int> pool;

        const size_t Iterations = 1000000;

        {
            std::println("new/delete with int");

            ScopedTimer timer;

            for (auto i = 0; i < Iterations; ++i)
                delete new int(i);
        }

        {
            std::println(" ObjectPool with int");

            ScopedTimer timer;

            for (auto i = 0; i < Iterations; ++i)
                pool.destroy(pool.construct(i));
        }
    }

    static void test_object_pool_11()
    {
        ObjectPool<Person> pool;

        const size_t Iterations = 1000000;

        {
            std::println("new/delete with Person");

            ScopedTimer timer;

            for (auto i = 0; i < Iterations; ++i) {
                delete new Person{ "Hans", "Mueller" , 30 }; // "Hans", "Mueller"
            }
        }

        {
            std::println("ObjectPool with Person");

            ScopedTimer timer;

            for (auto i = 0; i < Iterations; ++i) {
                pool.destroy(pool.construct(Person{ "Hans", "Mueller" , 30 }));
            }
        }
    }
}

void test_object_pool_fixed_size()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    ObjectPool_FixedSize_SimpleTest::test_object_pool_01();
    ObjectPool_FixedSize_SimpleTest::test_object_pool_02();
    ObjectPool_FixedSize_SimpleTest::test_object_pool_03();
    ObjectPool_FixedSize_SimpleTest::test_object_pool_04();

    ObjectPool_FixedSize_AdvancedTest::test_object_pool_10();
    ObjectPool_FixedSize_AdvancedTest::test_object_pool_11();
}

// ===========================================================================
// End-of-File
// ===========================================================================


