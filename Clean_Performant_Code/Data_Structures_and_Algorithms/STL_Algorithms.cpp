// ===========================================================================
// STL_Algorithms.cpp
// ===========================================================================

#include <algorithm>
#include <list>
#include <numeric>
#include <print>
#include <vector>

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
}

// =================================================================

void test_algorithms()
{
    using namespace STLAlgorithms;

    test_initializing();
    test_iterating();
    test_generating();
    test_generating_indices();
    test_sorting();
    test_finding();
}

// ===========================================================================
// End-of-File
// ===========================================================================
