// ===========================================================================
// LowLevelSTLAlgorithms.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <iostream>
#include <memory>
#include <numeric>
#include <print>
#include <print>
#include <set>
#include <vector>

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

            //Integer& operator=(const Integer& other) {
            //    std::println("operator=");
            //    m_value = other.m_value;
            //    return *this;
            //}

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

    constexpr size_t Size = 1000;
    constexpr size_t Iterations = 1'000'000;

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

                //for (auto it{ vec.begin()}; it != vec.end(); ++it) {
                //    std::print("{} ", (*it).get());
                //}
                //std::println();
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

                //for (auto it{ first }; it != last; ++it) {
                //    std::print("{} ", (*it).get());
                //}
                //std::println();

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

                //for (auto it{ vec.begin() }; it != vec.end(); ++it) {
                //    std::print("{} ", *it);
                //}
                //std::println();
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

                //for (auto it{ first }; it != last; ++it) {
                //    std::print("{} ", *it);
                //}
                //std::println();

                std::destroy(first, last);

                std::free(buffer);
            }
        }
    }
}

void low_level_memory_management_functions()
{
    using namespace LowLevel_MemoryFunctions_Functions;

    // introductionary examples
    Introductionary_Example_Using_Class_Integer::test_class_integer_standard_copy();
    Introductionary_Example_Using_Class_Integer::test_class_integer_uninitialized_copy();

    Introductionary_Example_Using_Class_StdString::test_class_std_string_standard_copy();
    Introductionary_Example_Using_Class_StdString::test_class_std_string_uninitialized_copy();

    // benchmark examples
    Benchmark_Example_Using_Class_Integer::test_class_integer_standard_copy();
    Benchmark_Example_Using_Class_Integer::test_class_integer_uninitialized_copy();

    Benchmark_Example_Using_Class_StdString::test_class_std_string_standard_copy();
    Benchmark_Example_Using_Class_StdString::test_class_std_string_uninitialized_copy();
}

// ===========================================================================
// End-of-File
// ===========================================================================
