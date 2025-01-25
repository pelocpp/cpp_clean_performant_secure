// ===========================================================================
// Benchmarking.cpp
// ===========================================================================

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// ===========================================================================
// Using Tool *Quick C++ Benchmark*
//
// Need to elimate some of the benchmarks intrinsic namespaces & declarations

#define BENCHMARK(function)   // just elimate this function call

namespace benchmark {

    struct State
    {
        struct StateIterator
        {
            StateIterator operator++ () { return *this; }
            auto operator*() const { return State{}; }

            bool operator== (const StateIterator& other) { return false; }
            bool operator!= (const StateIterator& other) { return !(*this == other); }
        };
    
        auto begin() { return StateIterator{}; }
        auto end() { return StateIterator{}; }
    };

    void DoNotOptimize (auto) {}
}


namespace PerformingBenchmarks {

    // ===========================================================================
    // Using Tool *Quick C++ Benchmark*
    // Getting Started Example

    namespace PerformingBenchmarkIntroduction {

        // =======================================================================
        // Introduction

        static void StringCreation(benchmark::State& state) {
            // Code inside this loop is measured repeatedly
            for (auto _ : state) {
                std::string created_string("hello");
                // Make sure the variable is not optimized away by compiler
                benchmark::DoNotOptimize(created_string);
            }
        }

        // Register the function as a benchmark
        BENCHMARK(StringCreation);

        static void StringCopy(benchmark::State& state) {
            // Code before the loop is not measured
            std::string x = "hello";
            for (auto _ : state) {
                std::string copy(x);
            }
        }
        BENCHMARK(StringCopy);
    }

    // ===========================================================================
    // Using Tool *Quick C++ Benchmark*
    // Comparing std::string and std::string_view

    namespace PerformingBenchmarkStringComparison {

        // ======================================================================
        // To-Copy Begin

        // prefix version using std::string
        static std::string prefix(const std::string& str) {
            if (str.length() >= 5) {
                // extract a part of string
                auto substr = str.substr(1, 4);  // substr is a std::string
                return substr;
            }
            return {};
        }

        // prefix version using std::string_view
        static std::string_view prefix(std::string_view str) {
            if (str.length() >= 5) {
                // extract a part of string
                auto substr = str.substr(1, 4);  // substr is a std::string_view
                return substr;
            }
            return {};
        }

        // Note: take care of SSO
        constexpr auto StringArgument1 = "1234567890";
        constexpr auto StringArgument2 = "The quick brown fox jumps over the lazy dog";
        constexpr auto StringArgument = StringArgument1;

        static void StdStringPrefix(benchmark::State& state) {

            for (auto _ : state) {

                std::string s{ StringArgument };
                auto result{ prefix(s) };

                benchmark::DoNotOptimize(result);
            }
        }

        BENCHMARK(StdStringPrefix); // register StdStringPrefix as a benchmark

        static void StdStringViewPrefix(benchmark::State& state) {

            for (auto _ : state) {

                std::string_view s{ StringArgument };
                auto result{ prefix(s) };

                benchmark::DoNotOptimize(result);
            }
        }

        BENCHMARK(StdStringViewPrefix); // register StdStringViewPrefix as a benchmark

        // To-Copy End
        // ======================================================================
    }
}

// =================================================================

void performance_performing_benchmarks()
{
    using namespace PerformingBenchmarks;
}

// ===========================================================================
// End-of-File
// ===========================================================================
