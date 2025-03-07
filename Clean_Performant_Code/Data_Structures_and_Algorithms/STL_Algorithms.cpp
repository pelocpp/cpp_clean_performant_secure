// ===========================================================================
// STL_Algorithms.cpp
// ===========================================================================

#include <algorithm>
#include <list>
#include <numeric>
#include <print>
#include <vector>

#include <iterator>

namespace STLAlgorithms {

    static void test_initializing()
    {
        std::vector<int> values(10);

        std::fill(
            values.begin(),
            values.end(),
            123
        );
    }

    static void test_iterating()
    {
        auto values = std::vector{ 9, 7, 1, 2, 3, 8, 10, 4, 5, 6 };

        std::for_each(
            values.begin(),
            values.end(), 
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    static void test_generating()
    {
        auto values = std::vector<int>(10);

        std::generate(
            values.begin(),
            values.end(),
            [n = 0]() mutable { ++n;  return 2 * n; }
        );

        std::for_each(
            values.begin(),
            values.end(),
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    static void test_generating_indices()
    {
        auto values = std::vector<int>(10);

        std::iota(
            values.begin(),
            values.end(),
            1
        );

        std::for_each(
            values.begin(),
            values.end(),
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    static void test_sorting()
    {
        auto values = std::vector{ 9, 7, 1, 2, 3, 8, 10, 4, 5, 6 };

        std::sort(
            values.begin(),
            values.end()
        );

        std::for_each(
            values.begin(),
            values.end(),
            [](auto i) { std::print("{} ", i); }
        );
        std::println();

        std::sort(
            values.begin(),
            values.end(),
            [](auto n, auto m) { return n > m; }
        );

        std::for_each(
            values.begin(),
            values.end(),
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    static void test_finding()
    {
        auto values = std::list{ 9, 7, 1, 2, 3, 8, 10, 4, 5, 6 };

        auto it = std::find(
            values.begin(),
            values.end(),
            3
        );

        if (it != values.end()) {
            std::println("{} ", *it);
        }
    }

    static void test_finding_binary()
    {
        auto values = std::list{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

        if (!is_sorted(values.begin(), values.end())) {
            std::println("Container is not sorted!");
            return;
        }

        auto elem{ 3 };

        auto found = std::binary_search(
            values.begin(),
            values.end(),
            elem
        );

        if (found) {
            std::println("Found {} ", elem);
        }
    }

    //static void test_finding_lower_bound()
    //{
    //    auto values = std::list{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

    //    if (!is_sorted(values.begin(), values.end())) {
    //        std::println("Container is not sorted!");
    //        return;
    //    }

    //    auto elem{ 3 };

    //    auto it = std::lower_bound(
    //        values.begin(),
    //        values.end(),
    //        elem
    //    );

    //    if (it != values.end()) {

    //        auto index{ std::distance(values.begin(), it) };
    //        std::println("Found element {} at position {}", *it, index);
    //    }
    //}

    //static void test_finding_upper_bound()
    //{
    //    auto values = std::list{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

    //    if (!is_sorted(values.begin(), values.end())) {
    //        std::println("Container is not sorted!");
    //        return;
    //    }

    //    auto elem{ 5 };

    //    auto it = std::upper_bound(
    //        values.begin(),
    //        values.end(),
    //        elem
    //    );

    //    if (it != values.end()) {

    //        auto index{ std::distance(values.begin(), it) };
    //        std::println("Found element {} at position {}", *it, index);
    //    }
    //}

    static void test_some_condition()
    {
        const auto values = std::vector{ 3, 2, 2, -1, 0, 2, 1 };

        auto noNegative = std::none_of(
            values.begin(),
            values.end(),
            [](int n) { return n < 0; }
        );

        if (noNegative) {
            std::println("Contains only positive numbers");
        }

        auto allPositive = std::all_of(
            values.begin(),
            values.end(),
            [](int n) { return n >= 0; }
        );

        if (allPositive) {
            std::println("Contains only positive numbers");
        }

        auto anyNegative = std::any_of(
            values.begin(),
            values.end(),
            [](int n) { return n < 0; }
        );

        if (anyNegative) {
            std::println("Contains at least one negative number");
        }
    }
}

// =================================================================

void test_algorithms()
{
    using namespace STLAlgorithms;

    //test_initializing();
    //test_iterating();
    //test_generating();
    //test_generating_indices();
    //test_sorting();
    //test_finding();

   // test_finding_binary();

    test_some_condition();
}

// ===========================================================================
// End-of-File
// ===========================================================================
