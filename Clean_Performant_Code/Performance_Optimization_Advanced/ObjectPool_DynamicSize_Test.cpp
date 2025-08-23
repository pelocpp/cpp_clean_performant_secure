// ===========================================================================
// ObjectPool_DynamicSize_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"
#include "../Person/Person.h"

#include "ObjectPool_DynamicSize.h"

#include <array>
#include <memory>
#include <print>

namespace ObjectPool_DynamicSize_SimpleTest {

    using namespace DynamicSizeObjectPool;

    static void test_object_pool_01()
    {
        using PersonPool = ObjectPool<Person>; 

        PersonPool pool{};

        auto person{ pool.acquireObject("Sepp", "Mueller", (size_t) 30) };

        std::println("{} {} has age {}",
            person->getFirstname(), person->getLastname(), person->getAge());

        std::println("Done.");
    }

    static void test_object_pool_02()
    {
        using PersonPool = ObjectPool<Person>;

        PersonPool pool{};

        auto sepp{ pool.acquireObject("Sepp", "Mueller", (size_t) 30) };
        auto hans{ pool.acquireObject("Hans", "Meier", (size_t) 40) };
        auto susi{ pool.acquireObject("Susi", "Wagner", (size_t) 50) };

        std::println("{} {} has age {}",
            sepp->getFirstname(), sepp->getLastname(), sepp->getAge());

        std::println("{} {} has age {}",
            hans->getFirstname(), hans->getLastname(), hans->getAge());

        std::println("{} {} has age {}",
            susi->getFirstname(), susi->getLastname(), susi->getAge());

        std::println("Done.");

        // Note:
        // 3 std::shared:ptr objects are going out of scope,
        // the according objects are put back in the list of free objects.
    }
}

namespace ObjectPool_DynamicSize_AdvancedTest {

    using namespace DynamicSizeObjectPool;

    static void test_object_pool_01()
    {
#ifdef _DEBUG
        static constexpr int Iterations = 1'000'000;        // debug
#else
        static constexpr int Iterations = 1'000'000;      // release
#endif

        ObjectPool<size_t> pool;

        std::println("Starting loop using Object Pool ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < Iterations; ++i) {
                auto ptr = pool.acquireObject(i);         // std::shared_ptr ... goes immediately out of scope
            }
        }

        std::println("Starting loop using standard new / delete ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < Iterations; ++i) {
                delete new size_t(i);
            }
        }
    }

    static void test_object_pool_02()
    {
#ifdef _DEBUG
        static constexpr int Iterations = 1'000'000;        // debug
#else
        static constexpr int Iterations = 1'000'000;      // release
#endif

        ObjectPool<Person> pool;

        std::println("Starting loop using Object Pool ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < Iterations; ++i) {
                auto ptr = pool.acquireObject(Person{ "Hans", "Mueller", 30 });         // std::shared_ptr ... goes immediately out of scope
            }
        }

        std::println("Starting loop using standard new / delete ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < Iterations; ++i) {
                delete new Person{ "Hans", "Mueller", 30 };
            }
        }
    }

    class ExpensiveObject
    {
    private:
        // an expensive data member
        static constexpr std::size_t Size1{ 100'000 };
        static constexpr std::size_t Size2{ 1'024 * 1'024 };
        static constexpr std::size_t Size3{ 2 * 1'024 * 1'024 };
        static constexpr std::size_t Size4{ 4 * 1'024 * 1'024 };

        std::array<double, Size2> m_data;

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

    static void test_object_pool_03()
    {
        const size_t NumberOfIterations{ 10'000 };

        MyPool pool;

        std::println("Starting loop using Object Pool ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ getExpensiveObject(pool) };
                processExpensiveObject(*object.get());
            }
        }

        std::println("Starting loop using standard new / delete ...");
        {
            ScopedTimer watch{};

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ std::make_unique<ExpensiveObject>() };
                processExpensiveObject(*object);
            }
        }
    }

    static void test_object_pool_04()
    {
        const size_t NumberOfIterations{ 1'000 };

        MyPool pool;

        std::println("Starting loop using pool...");

        {
            std::println("Using pool...");

            ScopedTimer watch{};

            std::vector<std::shared_ptr<ExpensiveObject>> listObjectsPool;

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ getExpensiveObject(pool) };
                processExpensiveObject(*object.get());
                listObjectsPool.push_back(object);
            }

            std::println("Done");
        }

        std::vector<std::unique_ptr<ExpensiveObject>> listObjectsUnique;

        std::println("Starting loop using new / delete ...");
        {
            std::println("Using std::make_unique...");

            ScopedTimer watch{};

            for (size_t i{ 0 }; i < NumberOfIterations; ++i) {
                auto object{ std::make_unique<ExpensiveObject>() };
                processExpensiveObject(*object);
                listObjectsUnique.push_back(std::move(object));
            }

            std::println("Done");
        }
    }
}

void test_object_pool_dynamic_size()
{
    ObjectPool_DynamicSize_SimpleTest::test_object_pool_01();
    ObjectPool_DynamicSize_SimpleTest::test_object_pool_02();

    ObjectPool_DynamicSize_AdvancedTest::test_object_pool_01();
    ObjectPool_DynamicSize_AdvancedTest::test_object_pool_02();
    ObjectPool_DynamicSize_AdvancedTest::test_object_pool_03();
    ObjectPool_DynamicSize_AdvancedTest::test_object_pool_04();
}

// ===========================================================================
// End-of-File
// ===========================================================================
