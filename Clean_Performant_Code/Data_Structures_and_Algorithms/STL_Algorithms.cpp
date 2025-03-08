// ===========================================================================
// STL_Algorithms.cpp
// ===========================================================================

#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <numeric>
#include <print>
#include <string>
#include <vector>

namespace STLAlgorithms {

    // -----------------------------------------------------------------------

    static void test_initializing()
    {
        std::vector<int> values(10);

        std::fill(
            values.begin(),
            values.end(),
            123
        );
    }

    // -----------------------------------------------------------------------

    static void test_iterating()
    {
        const auto values = std::vector{ 9, 7, 1, 2, 3, 8, 10, 4, 5, 6 };

        std::for_each(
            values.begin(),
            values.end(), 
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    // -----------------------------------------------------------------------

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

    // -----------------------------------------------------------------------

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

    // -----------------------------------------------------------------------

    static void test_copying()
    {
        std::vector<int> source(10, 123);
        std::vector<int> target(10);

        std::copy(
            source.begin(),
            source.end(),
            target.begin()
        );

        std::for_each(
            target.begin(),
            target.end(),
            [](auto i) { std::print("{} ", i); }
        );
        std::println();
    }

    // -----------------------------------------------------------------------

    static void test_transforming()
    {
        std::list<std::string> source {
            "1.5", "2.5", "3.5", "4.5", "5.5", "6.5", "7.5", "8.5", "9.5", "10.5"
        };
        
        std::vector<double> target;

        std::transform(
            source.begin(),
            source.end(),
            std::back_inserter(target),
            [](const auto& entry) {
                return std::stod(entry);
            }
        );

        std::for_each(
            target.begin(),
            target.end(),
            [](auto n) { std::print("{} ", n); }
        );
        std::println();
    }

    // -----------------------------------------------------------------------

    static std::string toString(const auto& vec) {
        return std::accumulate(
            vec.begin(),
            vec.end(),
            std::string{},
            [](const std::string& s, auto next) mutable {
                return s + std::to_string(next);
            }
        );
    }

    static void test_accumulate()
    {
        const auto values = std::vector{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        std::string s{ toString(values) };
        std::println("{}", s);
    }

    // -----------------------------------------------------------------------

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

    // -----------------------------------------------------------------------

    static void test_finding()
    {
        const auto values = std::list{ 9, 7, 1, 2, 3, 8, 10, 4, 5, 6 };

        auto it = std::find(
            values.begin(),
            values.end(),
            3
        );

        if (it != values.end()) {
            std::println("Found {} ", *it);
        }
    }

    // -----------------------------------------------------------------------

    static void test_finding_binary()
    {
        const auto values = std::list{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

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

    // -----------------------------------------------------------------------

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

    // -----------------------------------------------------------------------

    static void test_counting()
    {
        const auto values = std::list{ 3, 3, 2, 1, 3, 1, 3 };
        auto elem{ 3 };

        auto n = std::count(
            values.begin(),
            values.end(),
            elem
        );

        std::println("Found {} elements of value {}", elem, n);
    }

    // -----------------------------------------------------------------------

    static void test_counting_binary()
    {
        const auto values = std::list{ 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 };

        if (!is_sorted(values.begin(), values.end())) {
            std::println("Container is not sorted!");
            return;
        }

        auto elem{ 3 };

        auto range = std::equal_range(
            values.begin(),
            values.end(),
            elem
        );

        const auto& [begin, end] = range;
        auto count = std::distance(begin, end);
        std::println("Found {} elements of value {}", count, elem);
    }

    // -----------------------------------------------------------------------

    static auto some_func()
    {
        return 123;
    }

    static auto another_func()
    {
        return 456;
    }
    
    static void test_min_max_clamp()
    {
        const auto n = std::min(some_func(), another_func());
        std::println("Minimum: {}", n);

        const auto m = std::max(some_func(), another_func());
        std::println("Maximum: {}", m);

        const auto x = std::clamp(some_func(), 100, 200);
        std::println("Clamped Value: {}", x);

        const auto y = std::clamp(some_func(), 50, 100);
        std::println("Clamped Value: {}", y);

        const auto z = std::clamp(some_func(), 150, 200);
        std::println("Clamped Value: {}", z);
    }

    // -----------------------------------------------------------------------

    static void test_min_max()
    {
        const auto values = std::vector{ 4, 2, 1, 7, 3, 1, 5 };

        const auto [min, max] = std::minmax_element(values.begin(), values.end());

        std::println("Minimum: {}, Maximum: {}", *min, *max);

        auto minIndex = std::distance(values.begin(), min);
        std::println("minIndex: {}", minIndex);

        auto maxIndex = std::distance(values.begin(), max);
        std::println("minIndex: {}", maxIndex);
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
    test_copying();
    test_transforming();
    test_accumulate();
    test_sorting();
    test_finding();
    test_finding_binary();
    test_some_condition();
    test_counting();
    test_counting_binary();
    test_min_max_clamp();
    test_min_max();
}

// ===========================================================================
// End-of-File
// ===========================================================================
