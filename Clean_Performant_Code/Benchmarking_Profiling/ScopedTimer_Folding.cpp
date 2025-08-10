// ===========================================================================
// ScopedTimer_Folding.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <print>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>

// Note: use of keyword 'volatile' to prevent excessive optimizations!

namespace Benchmarking_Folding {

    // =======================================================================

    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {
        auto list = { values ... };
        volatile auto sum{ (size_t)0 };
        for (auto elem : list) {
            sum += elem;
        }
        return sum;
    }

    // =======================================================================

#ifdef _DEBUG
    static constexpr int Iterations = 10'000'000;        // debug
#else
    static constexpr int Iterations = 100'000'000;       // release
#endif

    static void FoldingSolution() {

        std::println("FoldingSolution");

        ScopedTimer watch{ ScopedTimer::Resolution::Micro };

        for (size_t i{}; i != Iterations; ++i) {

            volatile auto sum{ addFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }

    static void IterativeSolution() {

        std::println("IterativeSolution");

        ScopedTimer watch{ ScopedTimer::Resolution::Micro };

        for (size_t i{}; i != Iterations; ++i) {

            volatile auto sum{ addIterating(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) };
        }
    }
}

void benchmarking_folding2()
{
    using namespace Benchmarking_Folding;

    FoldingSolution();
    IterativeSolution();
}

// ===========================================================================
// End-of-File
// ===========================================================================
