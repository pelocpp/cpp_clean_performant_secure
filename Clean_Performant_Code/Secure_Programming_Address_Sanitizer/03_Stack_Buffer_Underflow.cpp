// ===========================================================================
// 03_Stack_Buffer_Underflow.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <print>
#include <thread>

static void test_03_stack_buffer_underflow_01()
{
    int subscript = -1;
    char buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    buffer[subscript] = 123; // Boom!
}

static void threadProcedure()
{
    int subscript = -1;
    char buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    buffer[subscript] = 123; // Boom!
}

static  void test_03_stack_buffer_underflow_02()
{
    std::thread t{ threadProcedure };
    t.join();
}

extern void test_03_stack_buffer_underflow()
{
    test_03_stack_buffer_underflow_01();
    test_03_stack_buffer_underflow_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
