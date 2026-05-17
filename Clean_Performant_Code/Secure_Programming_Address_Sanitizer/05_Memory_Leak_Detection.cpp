#include <print>


// GEHT NICHT

// ==21912==AddressSanitizer: detect_leaks is not supported on this platform.

void test_05_memory_leak_detection()
{
    int* buffer = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::println("buffer: {:#X}", reinterpret_cast<intptr_t>(buffer));

    // ... some complex body of code

    //delete[] buffer;

}