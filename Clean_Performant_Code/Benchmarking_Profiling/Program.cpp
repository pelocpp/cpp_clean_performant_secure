// ===========================================================================
// Program.cpp // Benchmarking // Profiling
// ===========================================================================

extern void performance_profiling_puzzle_10();
extern void performance_profiling_puzzle_11();

extern int benchmarking_folding(); 
extern int benchmarking_getting_started();
extern int benchmarking_lambda_vs_std_function();
extern int benchmarking_std_stringview_vs_std_string();

int main()
{
    //performance_profiling_puzzle_10();
    //performance_profiling_puzzle_11();
    
    benchmarking_folding();
    benchmarking_getting_started();
    benchmarking_lambda_vs_std_function();
    benchmarking_std_stringview_vs_std_string();
    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
