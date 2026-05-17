// ===========================================================================
// 07_Virtual_Base_Class_Destructor.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <memory>
#include <vector>

struct T
{
    T() : v(100) {}
    std::vector<int> v;
};

struct Base {
  //  virtual ~Base() = default;     // remove comment
};

struct Derived : public Base
{
    T t;
};

void test_07_virtual_base_class_destructor()
{
    Base* b = new Derived;
    delete b;  // Boom! 

    // oder

    std::unique_ptr<Base> b1 = std::make_unique<Derived>();
}

// ===========================================================================
// End-of-File
// ===========================================================================
