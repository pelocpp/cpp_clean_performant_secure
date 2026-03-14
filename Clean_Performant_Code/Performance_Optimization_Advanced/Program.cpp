// ===========================================================================
// Program.cpp // Performance Optimization Advanced
// ===========================================================================

extern void main_laundry();

extern void main_custom_allocator();
extern void main_standard_allocator();

extern void main_fixed_arena_controller();
extern void main_block_memory_manager();
extern void main_fixed_block_allocator();
extern void main_fixed_block_allocator_new_delete();

extern void main_object_pool_fixed_size();
extern void main_object_pool_dynamic_size();
extern void main_object_pool_thread_safe();

extern void main_cow_string();

extern void test_pmr_02();
extern void test_pmr_03();
extern void test_pmr_04();
extern void test_pmr_05();
extern void test_pmr_06();
extern void test_pmr_07();
extern void test_pmr_08();
extern void test_pmr_09();

int main()
{
    main_laundry();

    main_custom_allocator();
    main_standard_allocator();

    main_fixed_arena_controller();
    main_block_memory_manager();
    main_fixed_block_allocator();
    main_fixed_block_allocator_new_delete();

    main_object_pool_fixed_size();
    main_object_pool_dynamic_size();
    main_object_pool_thread_safe();

    main_cow_string();

    test_pmr_02();
    test_pmr_03();
    test_pmr_04();
    test_pmr_05();
    test_pmr_06();
    test_pmr_07();
    test_pmr_08();
    test_pmr_09();
    
    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
