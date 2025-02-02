// ===========================================================================
// Ranges.cpp
// ===========================================================================

#include <algorithm>
#include <cstddef>
#include <cstdlib>
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
            [](auto n) { std::print("{} ", n); }
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

        auto result{
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
        for (int n : std::views::iota(0) | std::views::take_while([](int y) { return y < 10; })) {
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
        for (auto prime : primeNumbers) {
            std::print("{} ", prime);
        }
    }

    // =======================================================================
    // Projections

    static void ranges_projections_01()
    {
        std::vector<int> numbers = { -8, 4, -6, -3, 2, -5, 10, 7, 1, -9 };

        // classic implementation: sort std::vector of integers by absolut value
        std::ranges::sort(
            numbers,
            [](auto lhs, auto rhs) {
                return std::abs(lhs) < std::abs(rhs); }
        );

        for (auto elem : numbers) {
            std::print("{} ", elem);
        }
    }

    static void ranges_projections_02()
    {
        std::vector<int> numbers = { -8, 4, -6, -3, 2, -5, 10, 7, 1, -9 };

        // using projection
        std::ranges::sort(
            numbers,
            std::ranges::less{},                  // comparison
            [](auto n) { return std::abs(n); }    // identity
        );

        for (auto elem : numbers) {
            std::print("{} ", elem);
        }
        std::println();
    }

    static void ranges_projections_03()
    {
        std::vector<int> numbers = { -8, 4, -6, -3, 2, -5, 10, 7, 1, -9 };

        // using defaulted projection and original value
        std::ranges::sort(
            numbers,
            {},
            {}
        );

        for (auto elem : numbers) {
            std::print("{} ", elem);
        }
        std::println();
    }

    struct Person
    {
        std::string m_name{};
        size_t      m_age{};
    };
}

namespace std
{
    using namespace Ranges;

    template <>
    struct formatter<Person>
    {
        constexpr auto parse(std::format_parse_context& ctx) {
            return ctx.begin();
        }

        auto format(const Person& person, std::format_context& ctx) const {
            return std::format_to(ctx.out(), "Name: {}, Age: {}", person.m_name, person.m_age);
        }
    };
}

namespace Ranges {

    // =======================================================================
    // Projections - Part II

    static void ranges_projections_04()
    {
        // sort struct by different fields
        std::vector<Person> persons{
            { "Sepp",  35 },
            { "Hans",  45 },
            { "Anton", 40 },
            { "Joe",   20 },
        };

       // ascending by name
       std::ranges::sort(
           persons,
           {}, 
           &Person::m_name
       );                     

       for (const auto& person : persons) {
           std::println("{} ", person);
       }
       std::println();

       // descending by name
       std::ranges::sort(
           persons,
           std::ranges::greater(),
           &Person::m_name
       );

       for (const auto& person : persons) {
           std::println("{} ", person);
       }
       std::println();

       // ascending by age
       std::ranges::sort(
           persons,
           {},
           &Person::m_age
       );
       
       for (const auto& person : persons) {
           std::println("{} ", person);
       }
       std::println();

       // descending by age
       std::ranges::sort(
           persons, 
           std::ranges::greater(),
           &Person::m_age
       );

       for (const auto& person : persons) {
           std::println("{} ", person);
       }
       std::println();
    }

    static void ranges_projections()
    {
        ranges_projections_01();
        ranges_projections_02();
        ranges_projections_03();
        ranges_projections_04();
    }

    // =======================================================================
    // Sentinels

    struct NegativeNumber
    {
        bool operator== (std::input_iterator auto iter) const {
            return *iter < 0;
        }
    };

    static void ranges_sentinels_01()
    {
        std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };

        std::ranges::transform(
            numbers.begin(),
            NegativeNumber{},   // <== sentinel
            numbers.begin(),
            [](const auto& n) { return n * n; }
        );

        for (auto elem : numbers) {
            std::print("{} ", elem);
        }
        std::println();
    }

    static void ranges_sentinels_02()
    {
        std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };

        std::ranges::for_each(
            numbers.begin(),
            NegativeNumber{},     // <== sentinel
            [] (auto n) { std::print("{} ", n); }
        );
        std::println();
    }

    static void ranges_sentinels_03()
    {
        std::vector<int> numbers{ 1, 2, 3, -4, 5, 6 };

        auto range{ 
            std::ranges::subrange{
                numbers.begin(),
                NegativeNumber{}   // <== sentinel
            }
        };

        for (auto elem : range) {
            std::print("{} ", elem);
        }
        std::println();
    }

    struct TerminatingZero
    {
        bool operator== (std::input_iterator auto iter) const {
            return *iter == '\0';
        }
    };

    static void ranges_sentinels_04()
    {
        const char* str = "Hello, World!";

        std::ranges::for_each(
            str,
            TerminatingZero{},
            [] (char c) { std::print("{} ", c); }
        );
    }

    static void ranges_sentinels()
    {
        ranges_sentinels_01();
        ranges_sentinels_02();
        ranges_sentinels_03();
        ranges_sentinels_04();
    }


    // =======================================================================
    // Dangling Iterators

    static std::vector<int> getData()
    {
        return std::vector<int> { 1, 2, 3, 4, 5 };
    }

    static void ranges_dangling_iterators_01()
    {
        //auto pos = std::ranges::find( getData(), 123); 

        //std::println("{} ", *pos);   // Error: You cannot dereference an operand of type 'std::ranges::dangling'
    }

    static void ranges_dangling_iterators_02()
    {
        const auto& values = getData();               // declare const lvalue reference

        auto value{ 3 };

        auto pos = std::ranges::find(values, value);

        if (pos == values.end()) {
            std::println("Value {} not found!", value);
        }
        else {
            std::println("Value {} found!", *pos);
        }
    }

    static void ranges_dangling_iterators()
    {
        ranges_dangling_iterators_01();
        ranges_dangling_iterators_02();
    }
}

// =================================================================

void ranges_clean_code_examples()
{
    using namespace Ranges;

    //comparison_iterators_vs_ranges();
    //ranges_example_concepts();
    //ranges_views();
    //ranges_range_adaptors();
    //ranges_composition_of_views();
    //ranges_lazy_evaluation();
    //ranges_bounded_vs_unbounded_range();
    //ranges_lazy_primes();
    //ranges_projections();
    //ranges_sentinels();
    ranges_dangling_iterators();
}

// ===========================================================================
// End-of-File
// ===========================================================================
