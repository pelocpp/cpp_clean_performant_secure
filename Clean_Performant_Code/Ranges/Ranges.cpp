// ===========================================================================
// Ranges.cpp
// ===========================================================================

#include <algorithm>
#include <iostream>
#include <list>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

namespace Ranges {

    // =======================================================================
    // Comparison Ranges vs. Traditional Iterators

    static void comparison_iterators() {

        std::vector<int> numbers{ 1, 4, 2, 7, 9, 3, 5 };

        std::sort(
            numbers.begin(),
            numbers.end()
        );
    }

    static void comparison_ranges() {

        std::vector<int> numbers{ 1, 4, 2, 7, 9, 3, 5 };

        std::ranges::sort(numbers);
    }

    static void comparison_iterators_vs_ranges()
    {
        comparison_iterators();
        comparison_ranges();
    }

    // =======================================================================
    // Use of Range Concepts
    // Beispiel: 
    // std::list ist ein bidirektionaler Range
    // Der Algorithmus std::sort erwartet einen Random Access Range
    // Compilation error random_access_range not satisfied

    static void stl_example_without_use_of_concepts()
    {
        //std::list<int> numbers{ 5, 6, 1, 3, 7 };

        //std::sort(
        //    numbers.begin(),
        //    numbers.end()
        //);
    }

    static void ranges_example_with_use_of_concepts()
    {
        //std::list<int> numbers{ 5, 6, 1, 3, 7 };
        //std::ranges::sort(numbers);
    }

    static void ranges_example_concepts()
    {
        stl_example_without_use_of_concepts();
        ranges_example_with_use_of_concepts();
    }

    // =======================================================================
    // Views

    static void ranges_views()
    {
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };

        auto view{ std::ranges::take_view{ numbers, 3 } };

        std::ranges::for_each(
            view,
            [] (auto n) { std::print("{} ", n); }
        );
    }

    // =======================================================================
    // Range Adaptors

    static void ranges_range_adaptors()
    {
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };

        auto result = numbers 
            | std::views::filter([](auto n) { return n % 2 == 0; })
            | std::views::transform([](auto n) { return n * 2; });

        for (auto n : result) {
            std::print("{} ", n);
        }
    }

    // =======================================================================
    // Function Composition, Pipelines

    static void ranges_composition_of_views_01()
    {
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto result {
            std::views::reverse(
                std::views::take(
                    std::views::transform(
                        std::views::filter(numbers, [](auto n) { return n % 2 == 0; }),
                        [](auto n) { return n * n; }
                    ),
                    4
                )
            ) 
        };

        std::ranges::for_each(
            result,
            [](auto n) { std::print("{} ", n); }
        );
    }

    // 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    // 2, 4, 6, 8, 10
    // 4, 16, 36, 64, 100
    // 4, 16, 36, 64
    // 64, 36, 16, 4

    static void ranges_composition_of_views_02()
    {
        std::vector<int> numbers = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        auto result = numbers | std::views::filter([](auto n) { return n % 2 == 0; })
            | std::views::transform([](auto n) { return n * n; })
            | std::views::take(4)
            | std::views::reverse;

        for (auto n : result) {
            std::print("{} ", n);
        }
    }

    static void ranges_composition_of_views()
    {
        ranges_composition_of_views_01();
        ranges_composition_of_views_02();
    }

    // =======================================================================
    // Lazy Evaluation

    static void ranges_lazy_evaluation()
    {
        auto lazySquares = 
            std::views::iota(1, 10) | std::views::transform([](auto i) { return i * i; });

        for (auto n : lazySquares) {
            std::print("{} ", n); // squares are calculated here
        }
    }

    // =======================================================================
    // Bounded vs. Unbounded (Infinite) Range

    static void ranges_bounded_vs_unbounded_range()
    {
        // use bounded std::views::iota with begin and end value
        for (int n : std::views::iota(0, 10)) {
            std::print("{} ", n);
        }
        std::println();
        
        // use infinite std::views::iota in combination with std::views::take
        for (int n : std::views::iota(0) | std::views::take(10)) { 
            std::print("{} ", n);
        }
        std::println();
        
        // use infinite std::views::iota in combination with std::views::take_while
        for (int n : std::views::iota(0) | std::views::take_while([] (int y) { return y < 10; })) {
            std::print("{} ", n);
        }
        std::println();

        // use infinite std::views::iota with begin value - this is an infinite loop
        //for (int n : std::views::iota(0)) {
        //    std::print("{} ", n);
        //}
    }

    // =======================================================================
    // Lazy Evaluation, Prime Numbers Example

    // predicate returns true if num is prime
    static bool isPrime(int num)
    {
        if (num <= 1) return false;
        if (num % 2 == 0) return false;

        for (int i = 2; i * i <= num; ++i) {
            if (num % i == 0) return false;
        }

        return true;
    }

    static void ranges_lazy_primes()
    {
        // generate an infinite range starting from 2
        auto numbers = std::ranges::iota_view{ 2 };

        // filter the numbers to keep only prime numbers and take the first 20 ones
        auto primeNumbers = numbers
            | std::views::filter(isPrime)
            | std::views::take(20);

        // print the first 20 prime numbers
        for (int prime : primeNumbers) {
            std::cout << prime << " ";
        }
    }
}

// =================================================================

void ranges_clean_code_examples()
{
    using namespace Ranges;

    comparison_iterators_vs_ranges();
    ranges_example_concepts();
    ranges_views();
    ranges_range_adaptors();
    ranges_composition_of_views();
    ranges_lazy_evaluation();
    ranges_bounded_vs_unbounded_range();
    ranges_lazy_primes();
}

// ===========================================================================
// End-of-File
// ===========================================================================
