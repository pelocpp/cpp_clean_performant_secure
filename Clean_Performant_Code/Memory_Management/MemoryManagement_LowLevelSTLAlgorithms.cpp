// ===========================================================================
// MemoryManagement_LowLevelSTLAlgorithms.cpp // Memory Management
// ===========================================================================

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>

#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include <numeric>
#include <print>
#include <set>
#include <vector>

// #define Verbose

namespace LowLevel_MemoryFunctions_Functions {

    namespace Class_Integer {

        class Integer
        {
        public:
            // c'tors
            Integer() : m_value{} {}
            Integer(int value) : m_value{ value } {}

            // getter
            int get() const { return m_value; }

#if defined (Verbose)
            Integer& operator=(const Integer& other) {
                std::println("operator=");
                m_value = other.m_value;
                return *this;
            }
#endif

        private:
            int m_value;
        };
    }

    // =======================================================================
    // Benchmark Functions

    namespace Introductionary_Example_Using_Class_Integer {

        using namespace Class_Integer;

        void static test_class_integer_standard_copy() {

            const size_t Size = 10;

            std::array<int, Size> numbers{};
            std::iota(numbers.begin(), numbers.end(), 1);

            std::vector<Integer> vec;
            vec.resize(Size);

            auto last = std::copy(
                numbers.begin(),
                numbers.end(),
                vec.begin()
            );

            for (auto it{ vec.begin() }; it != vec.end(); ++it) {
                std::print("{} ", (*it).get());
            }
            std::println();
        }

        void static test_class_integer_uninitialized_copy() {

            const size_t Size = 10;

            std::array<int, Size> numbers{};
            std::iota(numbers.begin(), numbers.end(), 1);

            void* buffer = std::malloc(Size * sizeof(Integer));
            if (buffer == nullptr) {
                return;
            }

            auto first = static_cast<Integer*>(buffer);

            Integer* last = std::uninitialized_copy(
                numbers.begin(),
                numbers.end(),
                first
            );

            for (auto it{ first }; it != last; ++it) {
                std::print("{} ", (*it).get());
            }
            std::println();

            std::free(buffer);
        }
    }

    namespace Introductionary_Example_Using_Class_StdString {

        void static test_class_std_string_standard_copy() {

            const size_t Size = 6;

            std::array<std::string, Size> strings{ "This", "is", "an", "array", "of", "strings" };

            std::vector<std::string> vec;
            vec.resize(Size);

            auto last = std::copy(
                strings.begin(),
                strings.end(),
                vec.begin()
            );

            for (auto it{ vec.begin() }; it != vec.end(); ++it) {
                std::print("{} ", *it);
            }
            std::println();
        }

        void static test_class_std_string_uninitialized_copy() {

            const size_t Size = 6;

            std::array<std::string, Size> strings{ "This", "is", "an", "array", "of", "strings" };

            void* buffer = std::malloc(Size * sizeof(std::string));
            if (buffer == nullptr) {
                return;
            }

            auto first = static_cast<std::string*>(buffer);

            std::string* last = std::uninitialized_copy(
                strings.begin(),
                strings.end(),
                first
            );

            for (auto it{ first }; it != last; ++it) {
                std::print("{} ", *it);
            }
            std::println();

            std::destroy(first, last);

            std::free(buffer);
        }
    }

    // =======================================================================
    // Benchmark Functions

    static constexpr size_t Size = 1000;
    static constexpr size_t Iterations = 1'000'000;

    namespace Benchmark_Example_Using_Class_Integer {

        using namespace Class_Integer;

        void static test_class_integer_standard_copy() {

            std::array<int, Size> numbers{};
            std::iota(numbers.begin(), numbers.end(), 1);

            ScopedTimer watch{};

            for (size_t i{}; i != Iterations; ++i) {

                std::vector<Integer> vec;
                vec.resize(Size);

                auto last = std::copy(
                    numbers.begin(),
                    numbers.end(),
                    vec.begin()
                );

#if defined (Verbose)
                for (auto it{ vec.begin()}; it != vec.end(); ++it) {
                    std::print("{} ", (*it).get());
                }
                std::println();
#endif
            }
        }

        void static test_class_integer_uninitialized_copy() {

            std::array<int, Size> numbers{};
            std::iota(numbers.begin(), numbers.end(), 1);

            ScopedTimer watch{};

            for (size_t i{}; i != Iterations; ++i) {

                void* buffer = std::malloc(Size * sizeof(Integer));
                if (buffer == nullptr) {
                    return;
                }

                auto first = static_cast<Integer*>(buffer);

                Integer* last = std::uninitialized_copy(
                    numbers.begin(),
                    numbers.end(),
                    first
                );

#if defined (Verbose)
                for (auto it{ first }; it != last; ++it) {
                    std::print("{} ", (*it).get());
                }
                std::println();
#endif

                std::free(buffer);
            }
        }
    }

    namespace Benchmark_Example_Using_Class_StdString {

        void static test_class_std_string_standard_copy() {

            const size_t Size = 6;
            std::array<std::string, Size> strings{ "This", "is", "an", "array", "of", "strings" };

            ScopedTimer watch{};

            for (size_t i{}; i != Iterations; ++i) {

                std::vector<std::string> vec;
                vec.resize(Size);

                auto last = std::copy(
                    strings.begin(),
                    strings.end(),
                    vec.begin()
                );

#if defined (Verbose)
                for (auto it{ vec.begin() }; it != vec.end(); ++it) {
                    std::print("{} ", *it);
                }
                std::println();
#endif
            }
        }

        void static test_class_std_string_uninitialized_copy() {

            const size_t Size = 6;

            std::array<std::string, Size> strings{ "This", "is", "an", "array", "of", "strings" };

            ScopedTimer watch{};

            for (size_t i{}; i != Iterations; ++i) {

                void* buffer = std::malloc(Size * sizeof(std::string));
                if (buffer == nullptr) {
                    return;
                }

                auto first = static_cast<std::string*>(buffer);

                std::string* last = std::uninitialized_copy(
                    strings.begin(),
                    strings.end(),
                    first
                );

#if defined (Verbose)
                for (auto it{ first }; it != last; ++it) {
                    std::print("{} ", *it);
                }
                std::println();
#endif

                std::destroy(first, last);

                std::free(buffer);
            }
        }
    }

    namespace Introductionary_Example_Uninitialized_Fill
    {
        void static test_uninitialized_fill() {

            const size_t Size = 2;

            void* buffer = std::malloc(Size * sizeof(std::string));
            if (buffer == nullptr) {
                return;
            }

            auto first = static_cast<std::string*>(buffer);

            auto last = first + Size;

            std::uninitialized_fill(first, last, std::string{ "std::uninitialized_fill example" });

            for (auto it{ first }; it != last; ++it) {
                std::println("{} ", *it);
            }

            std::destroy(first, last);

            std::free(buffer);
        }
    }

    namespace Introductionary_Example_Uninitialized_Move
    {
        void static test_uninitialized_move() {

            std::vector<std::string> src{ "Hello", "World!" };

            const size_t Size = src.size();

            void* buffer = std::malloc(Size * sizeof(std::string));
            if (buffer == nullptr) {
                return;
            }

            auto first = static_cast<std::string*>(buffer);

            std::uninitialized_move(src.begin(), src.end(), first);

            assert(src.at(0) == "");
            assert(src.at(1) == "");

            auto last = first + Size;

            for (auto it{ first }; it != last; ++it) {
                std::println("Target: {} ", *it);
            }

            std::destroy(first, last);

            std::free(buffer);
        }
    }

    namespace Introductionary_Value_and_Default_Construction
    {
        void static test_value_and_default_construction() {

            constexpr size_t Size = 5;

            using T = std::size_t;

            void* buffer = std::malloc(Size * sizeof(T));
            if (buffer == nullptr) {
                return;
            }
            
            auto first = static_cast<T*>(buffer);
            auto last = first + Size;

            std::println("Memory (std::malloc): ");
            for (auto it{ first }; it != last; ++it) {
                std::print("{:#x} ", *it);
            }
            std::println();

            // value construction: for POD types this means zero-initialization
            std::uninitialized_value_construct(first, last);

            std::println("uninitialized_value_construct: ");
            for (auto it{ first }; it != last; ++it) {
                std::print("{} ", *it);
            }
            std::println();

            // preparing the next example
            *first = 123;
            *(first + 1) = 456;
            *(first + 2) = 789;

            std::destroy(first, last);

            // default construction: for POD types this means no initialization
            std::uninitialized_default_construct(first, last);

            std::println("uninitialized_default_construct: ");
            for (auto it{ first }; it != last; ++it) {
                std::print("{} ", *it);
            }
            std::println();

            // Technically, the content is indeterminate values.
            // In practical terms the data will typically not be touched:
            // { 123, 456, 789, 0, 0 }

            std::destroy(first, last);

            std::free(buffer);
        }
    }
}

void memory_management_low_level_stl_functions()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    using namespace LowLevel_MemoryFunctions_Functions;

    // introductionary examples / copy construction
    Introductionary_Example_Using_Class_Integer::test_class_integer_standard_copy();
    Introductionary_Example_Using_Class_Integer::test_class_integer_uninitialized_copy();

    Introductionary_Example_Using_Class_StdString::test_class_std_string_standard_copy();
    Introductionary_Example_Using_Class_StdString::test_class_std_string_uninitialized_copy();

    // benchmark examples / copy construction
    Benchmark_Example_Using_Class_Integer::test_class_integer_standard_copy();
    Benchmark_Example_Using_Class_Integer::test_class_integer_uninitialized_copy();

    Benchmark_Example_Using_Class_StdString::test_class_std_string_standard_copy();
    Benchmark_Example_Using_Class_StdString::test_class_std_string_uninitialized_copy();

    // copy construction from a single value
    Introductionary_Example_Uninitialized_Fill::test_uninitialized_fill();

    // move construction from a single value
    Introductionary_Example_Uninitialized_Move::test_uninitialized_move();

    // Value and Default construction
    Introductionary_Value_and_Default_Construction::test_value_and_default_construction();
}

// ===========================================================================
// End-of-File
// ===========================================================================
