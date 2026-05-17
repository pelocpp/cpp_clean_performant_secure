#include <cstdlib>
#include <cstring>
#include <print>

static void test_04_double_free_01()
{
    char* buffer = (char*) std::malloc(10 * sizeof(char));
    std::memset(buffer, '0', 10);

    // ... some complex body of code

    free(buffer);

    // ... some complex body of code

    free(buffer);  // Boom!
}

static void test_04_double_free_02()
{
    int* buffer = new int[10] { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    std::println("buffer: {:#X}", reinterpret_cast<intptr_t>(buffer));

    // ... some complex body of code

    delete[] buffer;

    // ... some complex body of code

    delete[] buffer;
}

void test_04_double_free()
{
    test_04_double_free_01();
    test_04_double_free_02();
}
