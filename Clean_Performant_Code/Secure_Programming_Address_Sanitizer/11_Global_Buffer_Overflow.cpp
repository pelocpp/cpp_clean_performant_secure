// ===========================================================================
// 11_Global_Buffer_Overflow.cpp // Visual Studio Address Sanitizer
// ===========================================================================

// example3.cpp
// global-buffer-overflow error

// Run 4 different ways with the choice of one of these options:
//
// -g : Global
// -c : File static
// -f : Function static
// -l : String literal

#include <string.h>

struct C {
    static int array[10];
};

// normal global
static int global[10];

// class static
int C::array[10];

static int test_11_global_buffer_overflow_01()
{ 
    int first = 1;

    //char mode = { 'g' };
    //char mode = { 'c' };
    //char mode = { 'f' };
    char mode = { 'l' };



    switch (mode) {
    case 'g': return global[first * 11];     //Boom! simple global
    case 'c': return C::array[first * 11];   //Boom! class static
    case 'f':
    {
        static int array[10] = {};
        return array[first * 11];            //Boom! function static
    }
    case 'l':
        // literal global ptr created by compiler
        const char* str = "0123456789";
        return str[first * 11];              //Boom! .rdata string literal allocated by compiler
    }
    return 0;
}

void test_11_global_buffer_overflow()
{
    auto result = test_11_global_buffer_overflow_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
