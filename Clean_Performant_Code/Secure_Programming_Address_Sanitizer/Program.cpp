// ===========================================================================
// Program.cpp // Secure Programming // Visual Studio Address Sanitizer
// ===========================================================================

extern void test_01_basic_global_buffer_overflow();
extern void test_02_alloc_dealloc_mismatch();
extern void test_03_stack_buffer_underflow();
extern void test_04_double_free();
extern void test_05_memory_leak_detection();
extern void test_06_invalid_allocation_alignment();
extern void test_07_virtual_base_class_destructor();
extern void test_08_stack_use_after_scope();
extern void test_09_stack_use_after_return();
extern void test_10_global_buffer_overflow();

int main()
{
    //test_01_basic_global_buffer_overflow();
   //test_02_alloc_dealloc_mismatch();
    //test_03_stack_buffer_underflow();
    //test_04_double_free();
    //test_05_memory_leak_detection();
    //test_06_invalid_allocation_alignment();
   // test_07_virtual_base_class_destructor();
   // test_08_stack_use_after_scope();
    //test_09_stack_use_after_return();
    test_10_global_buffer_overflow();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
