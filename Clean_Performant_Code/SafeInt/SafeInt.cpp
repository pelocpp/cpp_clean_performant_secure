// ===========================================================================
// SafeInt.cpp
// ===========================================================================

#include "SafeInt.hpp"

#include <cstdint>
#include <print>
#include <vector>

namespace SafeFaculty {

    static size_t factorial (size_t x) {

        size_t ret{ 1 };

        for (size_t i{ 1 }; i <= x; ++i) {
            ret *= i;
        } 
        
        return ret;
    }

    static size_t factorial_safe(size_t x) {

        SafeInt<size_t> ret{ 1 };

        for (size_t i{ 1 }; i <= x; ++i) {
            ret *= i;
        }

        return ret;
    }

    static void test_factorial_unsafe()
    {
        for (size_t i{ 1 }; i != 30; ++i) {
            auto result{ factorial (i) };
            std::println("Factorial of {:2}: {}", i, result);
        }
    }

    static void test_factorial_safe()
    {
        try
        {
            for (size_t i{ 1 }; i != 30; ++i) {
                auto result{ factorial_safe(i) };
                std::println("Factorial of {:2}: {}", i, result);
            }
        }
        catch (const SafeIntException& ex)
        {
            if (ex.m_code == SafeIntArithmeticOverflow)
            {
                std::println("No more correct Factorial values available!");
            }
        }
    }
}

namespace SafeArithmetic {

    // sum the 16-bit signed integers stored in the values vector
    static int16_t sum(const std::vector<int16_t>& values)
    {
        int16_t result{};

        for (auto num : values)
        {
            result += num;
        }

        return result;
    }

    static int16_t sum_safe_hand_written(const std::vector<int16_t>& values)
    {
        int16_t result{};

        for (auto num : values)
        {
            //
            // Check for integer overflow *before* doing the sum
            //
            if (num > 0 && result > std::numeric_limits<int16_t>::max() - num)
            {
                throw std::overflow_error("Overflow in Sum function when adding a positive number.");
            }
            else if (num < 0 && result < std::numeric_limits<int16_t>::min() - num)
            {
                throw std::overflow_error("Overflow in Sum function when adding a negative number.");
            }
            result += num;
        }

        return result;
    }

    static int16_t sum_safe(const std::vector<int16_t>& values)
    {
        int16_t result{};

        for (auto num : values)
        {
            // Use SafeInt to check against integer overflow

            SafeInt<int16_t> sum{};    // = 0; <-- automatically init to 0

            for (auto num : values)
            {
                sum += num;   // <-- *automatically* checked against integer overflow!!
            }

            return sum;
        }

        return result;
    }

    // ===========================================================================

    static void test_maximum_int16_t()
    {
        std::print("Maximum value representable with int16_t: ");
        std::println("{}", std::numeric_limits<int16_t>::max());
    }

    static void test_sum_unsave() 
    {
        std::vector<int16_t> vec{ 30'000, 2'000, 700, 60, 7, 1 };   // 32'767 // change last 0 to 1
        auto result{ sum(vec) };
        std::println("Sum: {}", result);
    }

    static void test_sum_safe_hand_written()
    {
        std::vector<int16_t> vec{ 30'000, 2'000, 700, 60, 7, 1 };
        auto result{ sum_safe_hand_written(vec) };
        std::println("Sum: {}", result);
    }

    static void test_sum_safe()
    {
        std::vector<int16_t> vec{ 30'000, 2'000, 700, 60, 7, 1 };
        auto result{ sum_safe(vec) };
        std::println("Sum: {}", result);
    }

    static void test_sum_more_safe()
    {
        std::vector<int16_t> vec{  30'000, 2'000, 700, 60, 7, 1 };
        
        try
        {
            auto result{ sum_safe(vec) };
            std::println("Sum: {}", result);
        }
        catch (const SafeIntException& ex)
        {
            if (ex.m_code == SafeIntArithmeticOverflow)
            {
                std::println("SafeInt integer overflow exception correctly caught!");
            }
        }
    }
}

// =================================================================

void clean_code_arithmetic()
{
    using namespace SafeArithmetic;
    //test_maximum_int16_t();
    test_sum_unsave();
    //test_sum_safe_hand_written();
    //test_sum_safe();
    //test_sum_more_safe();

    //using namespace SafeFaculty;
    //test_factorial_unsafe();
    //test_factorial_safe();
}

// ===========================================================================
// End-of-File
// ===========================================================================
