// ===========================================================================
// 10_Stack_Use_After_Return.cpp // Visual Studio Address Sanitizer
// ===========================================================================

// geht nur mit:  /fsanitize-address-use-after-return 
// Enter /fsanitize-address-use-after-return in “Additional Options” under “Command Line”.

// Setzt man bei 

static volatile char* x = nullptr;

static void foo() {
    char stack_buffer[100];
    x = &stack_buffer[50];
}

static int test_10_stack_use_after_return_01() {

    foo();
    *x = 123; // Boom!

    return (*x == 123);
}

// C++ and templates


enum ReadOrWrite { Read = 0, Write = 1 };

struct S32 {
    char x[32];
};

template<class T>
static T* LeakStack() {
    T t[100];
    static volatile T* x;
    x = &t[0];
    return (T*)x;
}

template<class T>
static void StackUseAfterReturn(int Idx, ReadOrWrite w) {
    static T sink;
    T* t = LeakStack<T>();
    if (w)
        t[100 + Idx] = T();
    else
        sink = t[100 + Idx];
}


static void test_10_stack_use_after_return_02() {


    int kind = Read;   // oder Write

    switch (kind) {
        case 1: StackUseAfterReturn<char>(0, Read); break;
        case 2: StackUseAfterReturn<S32>(0, Write); break;
    }
}

void test_10_stack_use_after_return()
{
    // test_10_stack_use_after_return_01();
    test_10_stack_use_after_return_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
