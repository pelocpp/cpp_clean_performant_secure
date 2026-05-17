// ===========================================================================
// 01_Basic_Global_Buffer_Overflow.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <print>

int buffer[20];

void test_01_basic_global_buffer_overflow()
{
    std::println("Hello Global Buffer Overflow:");

    std::size_t n{ 20 };
    buffer[n] = 123; // Boom!
}

// ===========================================================================
// End-of-File
// ===========================================================================
