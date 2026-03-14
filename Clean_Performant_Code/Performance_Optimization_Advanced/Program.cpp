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

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
