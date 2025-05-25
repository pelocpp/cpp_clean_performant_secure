// ===========================================================================
// ScopedTimer_GettingStarted.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <print>
#include <string>

namespace Benchmarking_Getting_Started {

    static const size_t Iterations = 10'000'000;

    static void StringCreation() {

        std::println("StringCreation");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::string created_string("hello");
        }
    }

    static void StringCopy() {

        std::println("StringCopy");

        ScopedTimer watch{};

        std::string x = "hello";
        for (size_t i{}; i != Iterations; ++i) {
            std::string copy(x);
        }
    }
}

void benchmarking_getting_started()
{
    using namespace Benchmarking_Getting_Started;

    StringCreation();
    StringCopy();
}

// ===========================================================================
// End-of-File
// ===========================================================================
