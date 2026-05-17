// ===========================================================================
// 08_Stack_Use_After_Scope.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <array>
#include <functional>
#include <print>

// ---------------------------------------------------------------------------
// Example 1: Accessing simple nested local variable after its scope has ended

static int* gp = nullptr;
static bool b = true;

static int test_09_stack_use_after_scope_01() {

    if (b) {
        int x[5] = { };
        gp = x + 1;
    }

    return *gp;  // Boom!
}

// ---------------------------------------------------------------------------
// Example 2: Lambda Capture

static auto test_09_stack_use_after_scope_02() {

    std::function<int()> f;

    {
        int x = 0;
        f = [&x]() {
            return x;
        };
    }

    return f();  // Boom!
}

// ---------------------------------------------------------------------------
// Example 3: temporaries

struct A {

    A(const int& value) {
        m_ptr = &value;
    }

    void print() {
        std::println("{}", *m_ptr);
    }

    const int* m_ptr;
};

static void explicit_temp() {
    A a(123);     // the temp for 5 is no longer alive !!! UB
    a.print();
}

static void temp_from_conversion() {
    const int v = 456;
    A a(v);     // local v is alive but the temp for v is no longer alive !!! UB
    a.print();
}

static auto test_09_stack_use_after_scope_03() {

   explicit_temp();          // fails
   temp_from_conversion();   // correct
}

void test_08_stack_use_after_scope()
{
    // Example 1 
   // auto p = test_09_stack_use_after_scope_01();

    // Example 2 
    auto l = test_09_stack_use_after_scope_02();

    // Example 3
    // test_09_stack_use_after_scope_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
