// ===========================================================================
// ObjectPool_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../Person/Person.h"

#include "ObjectPool.h"

#include <array>
#include <chrono>
#include <memory>
#include <print>

namespace ObjectPool_SimpleTest {

    static void test_object_pool_01()
    {
        using PersonPool = ObjectPool<Person>;

        PersonPool pool{};

        auto person{ pool.acquireObject("Sepp", "Mueller", (size_t)30) };

        std::println("{} {} has age {}",
            person->getFirstname(), person->getLastname(), person->getAge());

        std::println("Done.");
    }
}


namespace ObjectPool_AdvancedTest {


    class ExpensiveObject
    {
    public:
        ExpensiveObject() : m_data{} { /* ... */ }
        virtual ~ExpensiveObject() = default;

        // Member functions to populate the object with specific information.
        // Member functions to retrieve the object data.
        // (not shown)

    private:
        // An expensive data member.
        std::array<double, 4 * 1024 * 1024> m_data;

        // Other data members (not shown)
    };

    using MyPool = ObjectPool<ExpensiveObject>;

    std::shared_ptr<ExpensiveObject> getExpensiveObject(MyPool& pool)
    {
        // Obtain an ExpensiveObject object from the pool.
        auto object{ pool.acquireObject() };

        // Populate the object. (not shown)

        return object;
    }

    void processExpensiveObject(ExpensiveObject& /*object*/)
    {
        // Process the object. (not shown)
    }

    void test_object_pool_02()
    {
        const size_t NumberOfIterations{ 500'000 };

        std::println("Starting loop using pool...");
        MyPool requestPool;
        auto start1{ std::chrono::steady_clock::now() };
        for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
            auto object{ getExpensiveObject(requestPool) };
            processExpensiveObject(*object.get());
        }
        auto end1{ std::chrono::steady_clock::now() };
        auto diff1{ end1 - start1 };
        std::println("{}", std::chrono::duration<double, std::milli>(diff1));

        std::println("Starting loop using new/delete...");
        auto start2{ std::chrono::steady_clock::now() };
        for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
            auto object{ std::make_unique<ExpensiveObject>() };
            processExpensiveObject(*object);
        }
        auto end2{ std::chrono::steady_clock::now() };
        auto diff2{ end2 - start2 };
        std::println("{}", std::chrono::duration<double, std::milli>(diff2));
    }
}

void test_object_pool()
{
    using namespace ObjectPool_SimpleTest;
    //test_object_pool_01();

    using namespace ObjectPool_AdvancedTest;
    test_object_pool_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
