// ===========================================================================
// Program.cpp // Performance Optimization Advanced
// ===========================================================================

extern void test_laundry();

extern void test_custom_allocator();
extern void test_standard_allocator();

extern void test_fixed_arena_controller();
extern void test_block_memory_manager();
extern void test_fixed_block_allocator();
extern void test_fixed_block_allocator_new_delete();

extern void test_object_pool_fixed_size();
extern void test_object_pool_dynamic_size();
//extern void test_object_pool_thread_safe();

int main()
{
    test_laundry();
    test_custom_allocator();
    test_standard_allocator();
    test_fixed_arena_controller();
    test_block_memory_manager();
    test_fixed_block_allocator();
    test_fixed_block_allocator_new_delete();
    test_object_pool_fixed_size();
    test_object_pool_dynamic_size();
    // test_object_pool_thread_safe();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
