// ===========================================================================
// ObjectPool_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"
#include "../Person/Person.h"

#include "ObjectPool.h"

#include <array>
#include <memory>
#include <print>

namespace ObjectPool_SimpleTest {

    static void test_object_pool_01()
    {
        using PersonPool = ObjectPool<Person>; 

        PersonPool pool{};

        auto person{ pool.acquireObject("Sepp", "Mueller", (size_t) 30) };

        std::println("{} {} has age {}",
            person->getFirstname(), person->getLastname(), person->getAge());

        std::println("Done.");
    }
}

namespace ObjectPool_AdvancedTest {

    class ExpensiveObject
    {
    private:
        // an expensive data member
        static constexpr std::size_t Size1{ 100'000 };
        static constexpr std::size_t Size2{ 1'024 * 1'024 };
        static constexpr std::size_t Size3{ 4 * 1'024 * 1'024 };

        std::array<double, Size1> m_data;

    public:
        ExpensiveObject() : m_data{} {}
        virtual ~ExpensiveObject() = default;
    };

    using MyPool = ObjectPool<ExpensiveObject>;

    static std::shared_ptr<ExpensiveObject> getExpensiveObject(MyPool& pool)
    {
        // obtain an ExpensiveObject object from the pool.
        auto object{ pool.acquireObject() };

        // using the object - not shown
        return object;
    }

    static void processExpensiveObject(ExpensiveObject& /*object*/)
    {
        // Process the object. (not shown)
    }

    static void test_object_pool_02()
    {
        const size_t NumberOfIterations{ 10'000 };

        MyPool pool;

        std::println("Starting loop using pool...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ getExpensiveObject(pool) };
                processExpensiveObject(*object.get());
            }
        }

        std::println("Starting loop using new/delete...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ std::make_unique<ExpensiveObject>() };
                processExpensiveObject(*object);
            }
        }
    }
}

void test_object_pool()
{
    // ObjectPool_SimpleTest::test_object_pool_01();
    ObjectPool_AdvancedTest::test_object_pool_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
