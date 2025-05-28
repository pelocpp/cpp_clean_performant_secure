// ===========================================================================
// ScopedTimer_StdArray_Constant_Initialization.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <execution>
#include <print>
#include <string>

namespace Benchmarking_StdArray_Constant_Initialization {

    static const size_t Iterations = 100'000;       // release

    static const int Size = 10'000;                 // release

    // need array in global data space, stack isn't suited for large objects
    std::array<double, Size> values{};

    static void ArrayConstantInitializationClassicForLoop() {

        std::println("ArrayConstantInitializationClassicForLoop");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            for (size_t k{}; k != values.size(); ++k) {
                values[k] = 123.0;
            }
        }
    }

    static void ArrayConstantInitializationIteratorBased() {

        std::println("ArrayConstantInitializationIteratorBased");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            for (auto it{ values.begin() }; it != values.end(); ++it) {
                *it = 123.0;
            }
        }
    }

    static void ArrayConstantInitializationStdFill() {

        std::println("ArrayConstantInitializationStdFill");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::fill(values.begin(), values.end(), 123.0);
        }
    }

    static void ArrayConstantInitializationStdFillParallelized() {

        std::println("ArrayConstantInitializationStdFillParallelized");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::fill(std::execution::par, values.begin(), values.end(), 123.0);
        }
    }

    static void ArrayConstantInitializationStdForEach() {

        std::println("ArrayConstantInitializationStdForEach");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::for_each(
                values.begin(),
                values.end(),
                [](auto& elem) { elem = 123.0; }
            );
        }
    }

    static void ArrayConstantInitializationRangeBasedForLoop() {

        std::println("ArrayConstantInitializationRangeBasedForLoop");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            for (auto& elem : values) {
                elem = 123.0;
            }
        }
    }

    static void ArrayConstantInitializationStdGenerate() {

        std::println("ArrayConstantInitializationStdGenerate");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::generate(
                values.begin(),
                values.end(),
                []() { return 123.0; }
            );
        }
    }
}

void benchmarking_std_array_constant_initialization()
{
    using namespace Benchmarking_StdArray_Constant_Initialization;

    ArrayConstantInitializationClassicForLoop();
    ArrayConstantInitializationIteratorBased();
    ArrayConstantInitializationStdFill();
    ArrayConstantInitializationStdFillParallelized();
    ArrayConstantInitializationStdForEach();
    ArrayConstantInitializationRangeBasedForLoop();
    ArrayConstantInitializationStdGenerate();
}

// ===========================================================================
// End-of-File
// ===========================================================================



