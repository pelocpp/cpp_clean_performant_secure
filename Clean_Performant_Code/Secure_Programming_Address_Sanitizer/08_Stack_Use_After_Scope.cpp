// ===========================================================================
// 09_Stack_Use_After_Scope.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <functional>
#include <print>

static int* gp = nullptr;

static bool b = true;

static int test_09_stack_use_after_scope_01() {

    if (b) {
        int x[5];
        gp = x + 1;
    }

    return *gp;  // Boom!
}

// example2.cpp
//  lambda capture
// stack-use-after-scope error


static auto test_09_stack_use_after_scope_02() {
    std::function<int()> f;
    {
        int x = 0;
        f = [&x]() {
            return x;  // Boom!
            };
    }
    return f();  // Boom!
}


struct IntHolder {
    explicit IntHolder(int* val = 0) : val_(val) {}
    ~IntHolder() {
        printf("Value: %d\n", *val_);  // Boom!
    }
    void set(int* val) { val_ = val; }
    int* get() { return val_; }

    int* val_;
};

static auto test_09_stack_use_after_scope_03(int value) {
    // It's incorrect to use "x" inside the IntHolder destructor,
        // because the lifetime of "x" ends earlier. Per the C++ standard,
        // local lifetimes end in reverse order of declaration.
    IntHolder holder;
    int x = value;
    holder.set(&x);
    return 0;
}


struct A {
    A(const int& v) {
        p = &v;
    }
    void print() {
        //std::cout << *p;
        std::print("{}", *p);
    }
    const int* p;
};

static void explicit_temp() {
    A a(5);     // the temp for 5 is no longer live;
    a.print();
}

static void temp_from_conversion() {
    double v = 5;
    A a(v);     // local v is no longer live.
    a.print();
}


static auto test_09_stack_use_after_scope_04() {

    explicit_temp();
    temp_from_conversion();
}



void test_08_stack_use_after_scope()
{
    auto p = test_09_stack_use_after_scope_01();

    //auto l = test_09_stack_use_after_scope_02();

    //test_09_stack_use_after_scope_03(3);  // geht nicht ...

    //test_09_stack_use_after_scope_04();
}

// ===========================================================================
// End-of-File
// ===========================================================================
