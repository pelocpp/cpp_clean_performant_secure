// ===========================================================================
// ScopedTimer_Folding2.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <print>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

namespace Benchmarking_Folding {

    // =======================================================================
    // generating random numbers

    static constexpr size_t NumInputs = 10;

    static std::vector<size_t> randomNumbers = [] {

        std::vector<size_t> numbers;
        numbers.resize(NumInputs);

        for (auto& n : numbers) {
            n = static_cast<size_t>(std::rand());
        }

        std::sort(
            numbers.begin(),
            numbers.end()
        );

        return numbers;
    } ();

    // =======================================================================

    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        auto list = { values ... };
        auto sum{ (size_t) 0 };
        for (auto elem : list) {
            sum += elem;
        }
        return sum;
    }

    // =======================================================================

    // const size_t Iterations = 10'000'000;

    static const size_t Iterations = 10'000'000;

    static void FoldingSolution() {

        std::println("FoldingSolution");

        size_t a0{ static_cast<size_t>(std::rand()) };
        size_t a1{ static_cast<size_t>(std::rand()) };
        size_t a2{ static_cast<size_t>(std::rand()) };
        size_t a3{ static_cast<size_t>(std::rand()) };
        size_t a4{ static_cast<size_t>(std::rand()) };
        size_t a5{ static_cast<size_t>(std::rand()) };
        size_t a6{ static_cast<size_t>(std::rand()) };
        size_t a7{ static_cast<size_t>(std::rand()) };
        size_t a8{ static_cast<size_t>(std::rand()) };
        size_t a9{ static_cast<size_t>(std::rand()) };

        // ScopedTimer watch{ ScopedTimer::Resolution::Nano };
        ScopedTimer watch{ ScopedTimer::Resolution::Micro };

        for (size_t i{}; i != Iterations; ++i) {

            volatile auto sum{ addFolding(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) };
        }
    }

    static void IterativeSolution() {

        std::println("IterativeSolution");

        size_t a0{ static_cast<size_t>(std::rand()) };
        size_t a1{ static_cast<size_t>(std::rand()) };
        size_t a2{ static_cast<size_t>(std::rand()) };
        size_t a3{ static_cast<size_t>(std::rand()) };
        size_t a4{ static_cast<size_t>(std::rand()) };
        size_t a5{ static_cast<size_t>(std::rand()) };
        size_t a6{ static_cast<size_t>(std::rand()) };
        size_t a7{ static_cast<size_t>(std::rand()) };
        size_t a8{ static_cast<size_t>(std::rand()) };
        size_t a9{ static_cast<size_t>(std::rand()) };

        // ScopedTimer watch{ ScopedTimer::Resolution::Nano };
        ScopedTimer watch{ ScopedTimer::Resolution::Micro };

        for (size_t i{}; i != Iterations; ++i) {

            volatile auto sum{ addIterating(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9) };
        }
    }
}

void benchmarking_folding()
{
    using namespace Benchmarking_Folding;

    FoldingSolution();
    IterativeSolution();
}

// ===========================================================================
// End-of-File
// ===========================================================================
