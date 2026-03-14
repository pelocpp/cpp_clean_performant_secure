// =====================================================================================
// PMR_05.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "PMR_DumpBuffer.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <exception>
#include <memory_resource>
#include <print>
#include <span>
#include <vector>

// =====================================================================================

static std::pmr::vector<std::uint8_t> splitToDigitsWrong(std::size_t number)
{
    // Note: 'resource' is a local variable - inside object 'digits'.
    // Object 'digits' is passed to the calling site - this cannot work
    // with a dangling pointer inside

    constexpr std::size_t NumBytes{ 32 };

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::uint8_t> digits{ &resource };

    do {
        std::uint8_t digit = number % 10;
        number /= 10;

        digits.push_back(digit);

    } while (number != 0);

    std::reverse(digits.begin(), digits.end());

    return digits;
}

static void test_pmr_05_01_wrong()
{
    try {
        std::size_t number{ 12345 };

        std::pmr::vector<std::uint8_t> digits{ splitToDigitsWrong(number) };

        for (std::size_t i{}; auto digit : digits) {
            std::println("{}: {}", i, digit);
            ++i;
        }
    }
    catch (std::exception ex) {
        std::println("std::exception: {}", ex.what());
    }
}

// =====================================================================================
// passing both the container and the resource object

static std::pmr::vector<std::uint8_t> splitToDigitsCorrect(std::size_t number, std::pmr::memory_resource* res)
{
    std::pmr::vector<std::uint8_t> digits{ res };

    do {
        std::uint8_t digit = number % 10;
        number /= 10;

        digits.push_back(digit);
    } while (number != 0);

    std::reverse(digits.begin(), digits.end());

    return digits;
}

static void test_pmr_05_02_correct()
{
    constexpr std::size_t NumBytes{ 32 };

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::size_t number{ 12345 };

    std::pmr::vector<std::uint8_t> digits{ splitToDigitsCorrect(number, &resource) };

    for (std::size_t i{}; auto digit : digits) {
        std::println("{}: {}", i, digit);
        ++i;
    }
}

// =====================================================================================

void test_pmr_05()
{
    // test_pmr_05_01_wrong();    // crashes by design
    test_pmr_05_02_correct();
}

// =====================================================================================
// End-of-File
// =====================================================================================
