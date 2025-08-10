// ===========================================================================
// Program.cpp // Performance Optimization Advanced
// ===========================================================================

extern void test_custom_allocator();
extern void test_fixed_arena_controller();
extern void test_block_memory_manager();
extern void test_fixed_block_allocator();
extern void test_object_pool();

int main()
{
    //test_custom_allocator();
    //test_fixed_arena_controller();
    //test_block_memory_manager();
    test_fixed_block_allocator();
    //test_object_pool();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
