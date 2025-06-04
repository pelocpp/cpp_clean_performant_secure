// ===========================================================================
// TestObjectPool.cpp // Best Practices
// ===========================================================================

#include "../Person/Person.h"

#include "ObjectPool.h"

#include <array>
#include <chrono>
#include <memory>
#include <print>

static void object_pool_best_practices_01()
{
    using PersonPool = ObjectPool<Person>;

    PersonPool pool{};

    auto person { pool.acquireObject( "Sepp", "Mueller", (size_t) 30 )};

    std::println("Done.");
}

void object_pool_best_practices()
{
    object_pool_best_practices_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================

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

void main_________()
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
