// ===========================================================================
// Program.cpp // Ranges
// ===========================================================================

namespace Ranges_Demo
{
    extern void  comparison_iterators_vs_ranges();
    extern void  ranges_example_concepts();
    extern void  ranges_views();
    extern void  ranges_range_adaptors_last_example_improved();
    extern void  ranges_range_adaptors();
    extern void  ranges_range_adaptors_classic();
    extern void  ranges_composition_of_views();
    extern void  ranges_lazy_evaluation();
    extern void  ranges_eager_evaluation();
    extern void  ranges_bounded_vs_unbounded_range();
    extern void  ranges_lazy_primes();
    extern void  ranges_projections();
    extern void  ranges_sentinels();
    extern void  ranges_dangling_iterators();
    extern void  ranges_keys_view_and_values_view();
    extern void  ranges_common_view();
    extern void  ranges_all_of_any_of_none_of();
    extern void  ranges_example_variant();
    extern void  ranges_example_unordered_map();
    extern void  ranges_example_trim_alpha_tolower();
}

int main()
{
    using namespace Ranges_Demo;

    comparison_iterators_vs_ranges();
    ranges_example_concepts();
    ranges_views();
    ranges_range_adaptors_last_example_improved();
    ranges_range_adaptors();
    ranges_range_adaptors_classic();
    ranges_composition_of_views();
    ranges_lazy_evaluation();
    ranges_eager_evaluation();
    ranges_bounded_vs_unbounded_range();
    ranges_lazy_primes();
    ranges_projections();
    ranges_sentinels();
    ranges_dangling_iterators();
    ranges_keys_view_and_values_view();
    ranges_common_view();
    ranges_all_of_any_of_none_of();
    ranges_example_variant();
    ranges_example_unordered_map();
    ranges_example_trim_alpha_tolower();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
