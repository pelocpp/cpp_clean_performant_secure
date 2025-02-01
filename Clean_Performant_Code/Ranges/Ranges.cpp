// ===========================================================================
// Ranges.cpp
// ===========================================================================

#include <algorithm>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
#include <list>

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

    static int stl_example_without_use_of_concepts()
    {
        //std::list<int> numbers{ 5, 6, 1, 3, 7 };

        //std::sort(
        //    numbers.begin(),
        //    numbers.end()
        //);
    }

    static int ranges_example_with_use_of_concepts()
    {
        //std::list<int> numbers{ 5, 6, 1, 3, 7 };

        //std::ranges::sort(numbers);
    }

    static void ranges_example_concepts()
    {
        stl_example_without_use_of_concepts();
        ranges_example_with_use_of_concepts();
    }
}

// =================================================================

void ranges_clean_code_examples()
{
    using namespace Ranges;

    comparison_iterators_vs_ranges();
    ranges_example_concepts();

}

// ===========================================================================
// End-of-File
// ===========================================================================
