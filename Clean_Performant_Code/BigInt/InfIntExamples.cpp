// ===========================================================================
// InfIntExamples.cpp // Arbitrary-sized integer class for C++
// ===========================================================================

#include "InfInt.h"

#include <cstddef>
#include <format>
#include <print>
#include <string>

// ===========================================================================
// adding std::println support to class InfInt

namespace std {

    // formatter for class InfInt
    template <>
    struct std::formatter<InfInt> {
        constexpr auto parse(std::format_parse_context & ctx) {
            return ctx.begin();
        }

        auto format(const InfInt& big, std::format_context & ctx) const {
            std::string s = big.toString();
            return std::format_to(ctx.out(), "{}", s);
        }
    };
}

// ===========================================================================
// adding Faculty support

// function to find the faculty of given number
static InfInt factorial(int n)
{
    InfInt res{ 1 };

    for (int i{ 2 }; i <= n; ++i) {
        res *= i;
    }

    return res;
}

// template member method 'factorial' can be used
// both for size_t and InfInt types
class BigFactorial
{
public:
    template <typename T>
    static T factorial(const T& n)
    {
        if (n == static_cast<T> (1)) {
            return static_cast<T> (1);
        }
        else {
            return n * factorial(n - static_cast<T>(1));
        }
    }
};

// ===========================================================================
// InfInt examples

namespace InfIntExamples {

    static void infint_example_01() {

        InfInt n = 1234567890;
        std::println("{}", n);
    }

    static void infint_example_02() {

        // testing addition
        InfInt n1{ "11111111" };
        InfInt n2{ "22222222" };
        InfInt n = n1 + n2;
        std::println("{} + {} = {}", n1, n2, n);
    }

    static void infint_example_03() {

        // testing addition
        InfInt n1 = InfInt{ "999999999999999999999999999999" };
        InfInt n2 = InfInt{ "1" };
        InfInt n = n1 + n2;
        std::println("{} + {} = {}", n1, n2, n);
    }
    
    static void infint_example_04_faculty() {

        // testing faculty
        for (int n{ 2 }; n < 51; ++n)
        {
            InfInt f{ factorial(n) };
            std::println("Faculty of {:2}: {}", n, f);
        }
    }

    static void infint_example_05_faculty() {

        for (std::size_t n{ 2 }; n < 51; ++n)
        {
            std::size_t f{ BigFactorial::factorial<std::size_t>(n) };
            std::println("Faculty of {:2}: {}", n, f);
        }
    }

    static void infint_example_06_faculty() {

        for (std::size_t n{ 2 }; n < 51; ++n)
        {
            InfInt f{ BigFactorial::factorial<InfInt>(n) };
            std::println("Faculty of {:2}: {}", n, f);
        }
    }
}

void infint_examples()
{
    using namespace InfIntExamples;

    infint_example_01();
    infint_example_02();
    infint_example_03();

    infint_example_04_faculty();
    infint_example_05_faculty();
    infint_example_06_faculty();
}

// ===========================================================================
// End-of-File
// ===========================================================================
