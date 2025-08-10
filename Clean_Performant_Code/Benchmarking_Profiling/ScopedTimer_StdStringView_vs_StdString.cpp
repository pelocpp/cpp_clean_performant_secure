// ===========================================================================
// ScopedTimer_StdStringView_vs_StdString.cpp
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <print>
#include <string>
#include <string_view>

namespace Benchmarking_Std_Stringview_vs_Std_String {

#ifdef _DEBUG
    static constexpr int Iterations = 1'000'000;        // debug
#else
    static constexpr int Iterations = 100'000'000;      // release
#endif


    // Note: take care of SSO
    static auto StringArgument1 = "1234567890";
    static auto StringArgument2 = "The quick brown fox jumps over the lazy dog";
    static auto StringArgument = StringArgument1;

    // prefix version using std::string
    static std::string prefix(const std::string& str) {
        if (str.length() >= 5) {
            // extract a part of string
            auto substr = str.substr(1, 4);    // substr is a std::string
            return substr;
        }
        return {};
    }

    // prefix version using std::string_view
    static std::string_view prefix(std::string_view str) {
        if (str.length() >= 5) {
            // extract a part of string
            auto substr = str.substr(1, 4);    // substr is a std::string_view
            return substr;
        }
        return {};
    }

    static void StdStringPrefix() {

        std::println("StdStringPrefix");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::string s{ StringArgument };
            auto result{ prefix(s) };
        }
    }

    static void StdStringViewPrefix() {

        std::println("StdStringViewPrefix");

        ScopedTimer watch{};

        for (size_t i{}; i != Iterations; ++i) {
            std::string_view s{ StringArgument };
            auto result{ prefix(s) };
        }
    }
}

void benchmarking_std_stringview_vs_std_string()
{
    using namespace Benchmarking_Std_Stringview_vs_Std_String;

    StdStringPrefix();
    StdStringViewPrefix();
}

// ===========================================================================
// End-of-File
// ===========================================================================
