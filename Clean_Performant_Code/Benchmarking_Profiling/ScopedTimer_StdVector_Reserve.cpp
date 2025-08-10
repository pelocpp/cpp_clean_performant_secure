// ===========================================================================
// ScopedTimer_StdVector_Reserve.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <print>
#include <vector>

namespace Benchmarking_Std_Vector_Reserve {

#ifdef _DEBUG
    static constexpr int Iterations = 100'000;        // debug
#else
    static constexpr int Iterations = 1'000'000;      // release
#endif

    static constexpr int VectorSize = 100;

    static void VecPushBack() {

        std::println("VecPushBack");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::vector<char> vec;
            for (auto i = 0; i < VectorSize; ++i)
                vec.push_back(' ');
        }
    }

    static void VecReserve() {

        std::println("VecReserve");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::vector<char> vec;
            vec.reserve(VectorSize);
            for (auto i = 0; i < VectorSize; ++i)
                vec.push_back(' ');
        }
    }
}

void benchmarking_std_vector_reserve()
{
    using namespace Benchmarking_Std_Vector_Reserve;

    VecPushBack();
    VecReserve();
}

// ===========================================================================
// End-of-File
// ===========================================================================
