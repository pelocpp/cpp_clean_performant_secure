// ===========================================================================
// 09_Stack_Use_After_Return.cpp // Visual Studio Address Sanitizer
// ===========================================================================

// Just works with:  /fsanitize-address-use-after-return 
// Enter /fsanitize-address-use-after-return in "Additional Options" under "Command Line".

static volatile char* x = nullptr;

static void foo() {
    char stack_buffer[100]{};
    x = &stack_buffer[50];
}

static int test_09_stack_use_after_return_01() {

    foo();

    *x = 123; // Boom!

    return (*x == 123);
}

void test_09_stack_use_after_return()
{
    test_09_stack_use_after_return_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
