// ===========================================================================
// MemoryManagement_Placement_New.cpp // Memory Management
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"
#include "../Person/Person.h"

#include <algorithm>
#include <memory>
#include <print>
#include <string>

namespace Placement_New {

    // =======================================================================
    // Placement New

    unsigned char alignas (std::max_align_t) g_memory[sizeof(Person)];

    static void test_placement_new_01() {

        std::println("sizeof (Person): {}", sizeof(Person));

        auto* person = ::new (g_memory) Person{ "Sepp", "Mueller", static_cast<size_t>(30) };

        person->~Person();
    }

    static void test_placement_new_02() {

        auto* memory = std::malloc(sizeof(Person));
        auto* person = ::new (memory) Person{ "Sepp", "Mueller", static_cast<size_t>(30) };

        person->~Person();
        std::free(memory);
    }

    static void test_placement_new_03() {

        auto* memory = std::malloc(sizeof(Person));
        auto* person = reinterpret_cast<Person*>(memory);

        std::uninitialized_fill_n(person, 1, Person{ "Sepp", "Mueller", static_cast<size_t>(30) });

        std::destroy_at(person);
        std::free(memory);
    }

    static void test_placement_new_04() {

        auto* memory = std::malloc(sizeof(Person));
        auto* person = reinterpret_cast<Person*>(memory);

        std::construct_at(person, Person{ "Sepp", "Mueller", static_cast<size_t>(30) }); // C++20

        std::destroy_at(person);
        std::free(memory);
    }
}

// =======================================================================
// Placement New: Performance Comparison

namespace Placement_New_Example {

#ifdef _DEBUG
    constexpr std::size_t DataSize = 50'000;
    constexpr std::size_t NumIterations = 100;
#else
    constexpr std::size_t DataSize = 100'000;
    constexpr std::size_t NumIterations = 300;
#endif

    static void test_placement_new_example_00()
    {
        // remove comments from class Person

        using T = Person;

        Person hans{ "Hans", "Albert", 33 };

        std::size_t size = 5;

        T* elems = static_cast<T*> (std::malloc(5 * sizeof(T)));

        std::uninitialized_fill(elems, elems + size, hans);

        Person sepp{ "Sepp", "Meier", 33 };

        std::fill(elems, elems + size, sepp);

        std::destroy(elems, elems + size);
        std::free(elems);
    }

    namespace BigData_Classic_Implementation
    {
        template <typename T>
        class BigData
        {
        private:
            T* m_elems{};
            std::size_t m_size{};

        public:
            // c'tor(s) / d'tor
            BigData() = default;

            BigData(std::size_t size, const T& init)
                : m_elems{ new T[size] }, m_size{ size }
            {
                std::fill(m_elems, m_elems + m_size, init);
            }

            ~BigData()
            {
                delete[] m_elems;
            }

            void print()
            {
                for (std::size_t i{}; i != m_size; ++i) {
                    std::println("{:02}: {}", i, m_elems[i]);
                }
            }
        };
    }

    static void test_placement_new_example_01()
    {
        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Implementation::BigData<int> data{ DataSize, 123 };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Implementation::BigData<std::string> data{ DataSize, std::string{ "C++ Memory Management" } };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Implementation::BigData<Person> data{ DataSize, Person{ "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", static_cast<size_t>(30) } };
            }
        }
        std::println();
    }

    namespace BigData_Classic_Improved_Implementation
    {
        template <typename T>
        class BigData
        {
        private:
            T* m_elems{};
            std::size_t m_size{};

        public:
            // c'tor(s)
            BigData() = default;

            BigData(std::size_t size, const T& init)
            {
                m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
                m_size = size;

                for (auto pBegin = m_elems; pBegin != m_elems + m_size; ++pBegin) {
                    ::new (static_cast<void*>(pBegin)) T{ init };
                }
            }

            ~BigData()
            {
                std::for_each(
                    m_elems,
                    m_elems + m_size,
                    [](const T& obj) { obj.~T(); }
                );

                std::free(m_elems);
            }

            void print()
            {
                for (std::size_t i{}; i != m_size; ++i) {
                    std::println("{:02}: {}", i, m_elems[i]);
                }
            }
        };
    }

    static void test_placement_new_example_02()
    {
        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Improved_Implementation::BigData<int> data{ DataSize, 123 };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Improved_Implementation::BigData<std::string> data{ DataSize, "C++ Memory Management" };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Implementation::BigData<Person> data{ DataSize, Person{ "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", static_cast<size_t>(30) } };
            }
        }
        std::println();
    }

    namespace BigData_Classic_More_Improved_Implementation
    {
        template <typename T>
        class BigData
        {
        private:
            T* m_elems{};
            std::size_t m_size{};

        public:
            // c'tor(s)
            BigData() = default;

            BigData(std::size_t size, const T& init)
            {
                m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
                m_size = size;
                std::uninitialized_fill(m_elems, m_elems + m_size, init);
            }

            ~BigData()
            {
                std::destroy(m_elems, m_elems + m_size);
                std::free(m_elems);
            }

            void print()
            {
                for (std::size_t i{}; i != m_size; ++i) {
                    std::println("{:02}: {}", i, m_elems[i]);
                }
            }
        };
    }

    static void test_placement_new_example_03()
    {
        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Improved_Implementation::BigData<int> data{ DataSize, 123 };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Improved_Implementation::BigData<std::string> data{ DataSize, "C++ Memory Management" };
            }
        }

        {
            ScopedTimer watch;
            for (size_t i{}; i != NumIterations; ++i) {
                volatile BigData_Classic_Implementation::BigData<Person> data{ DataSize, Person{ "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", static_cast<size_t>(30) } };
            }
        }
        std::println();
    }
}

// =======================================================================
// Placement New: Quickbench Examples

namespace Placement_New_Example_Quickbench {

    // To-Copy Begin
    // ======================================================================

#if defined (QUICKBENCH_BEGIN)

    constexpr std::size_t Max = 10;

    class Person
    {
    private:
        std::string m_first;
        std::string m_last;
        std::size_t m_age;

    public:
        Person() : m_first{}, m_last{}, m_age{} {}

        Person(std::string first, std::string last, size_t age)
            : m_first{ first }, m_last{ last }, m_age{ age }
        {
        }
    };

    namespace BigData_Classic_Implementation
    {
        template <typename T>
        class BigData
        {
        private:
            T* m_elems{};
            std::size_t m_size{};

        public:
            // c'tor(s) / d'tor
            BigData() = default;

            BigData(std::size_t size, const T& init)
                : m_elems{ new T[size] }, m_size{ size }
            {
                std::fill(m_elems, m_elems + m_size, init);
            }

            ~BigData()
            {
                delete[] m_elems;
            }
        };
    }

    static void BigDataClassicInt(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_Implementation::BigData<int> data{ Max, 123 };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataClassicInt);

    static void BigDataClassicString(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_Implementation::BigData<std::string> data{ Max, std::string{ "C++ Memory Management" } };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataClassicString);

    static void BigDataClassicPerson(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_Implementation::BigData<Person> data{ Max, Person{ "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", static_cast<size_t>(30) } };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataClassicPerson);

    //============================================================

    namespace BigData_Classic_More_Improved_Implementation
    {
        template <typename T>
        class BigData
        {
        private:
            T* m_elems{};
            std::size_t m_size{};

        public:
            // c'tor(s)
            BigData() = default;

            BigData(std::size_t size, const T& init)
            {
                m_elems = static_cast<T*> (std::malloc(size * sizeof(T)));
                m_size = size;
                std::uninitialized_fill(m_elems, m_elems + m_size, init);
            }

            ~BigData()
            {
                std::destroy(m_elems, m_elems + m_size);
                std::free(m_elems);
            }
        };
    }

    static void BigDataImprovedInt(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_More_Improved_Implementation::BigData<int> data{ Max, 123 };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataImprovedInt);

    static void BigDataImprovedString(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_More_Improved_Implementation::BigData<std::string> data{ Max, std::string{ "C++ Memory Management" } };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataImprovedString);

    static void BigDataImprovedPerson(benchmark::State& state) {
        // Code inside this loop is measured repeatedly
        for (auto _ : state) {
            volatile BigData_Classic_More_Improved_Implementation::BigData<Person> data{ Max, Person{ "AAAAAAAAAAAAAAAAAAAA", "BBBBBBBBBBBBBBBBBBBB", static_cast<size_t>(30) } };
            // Make sure the variable is not optimized away by compiler
            benchmark::DoNotOptimize(data);
        }
    }
    // Register the function as a benchmark
    BENCHMARK(BigDataImprovedPerson);

#endif

    // To-Copy End
    // ======================================================================
}

void memory_management_placement_new()
{
    Placement_New::test_placement_new_01();
    std::println();
    Placement_New::test_placement_new_02();
    std::println();
    Placement_New::test_placement_new_03();
    std::println();
    Placement_New::test_placement_new_04();
    std::println();

    Placement_New_Example::test_placement_new_example_00();
    Placement_New_Example::test_placement_new_example_01();
    Placement_New_Example::test_placement_new_example_02();
    Placement_New_Example::test_placement_new_example_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
