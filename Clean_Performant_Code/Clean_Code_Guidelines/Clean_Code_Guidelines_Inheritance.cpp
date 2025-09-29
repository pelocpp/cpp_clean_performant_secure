// ===========================================================================
// Clean_Code_Guidelines_Inheritance.cpp
// ===========================================================================

#include <cassert>
#include <print>

namespace GuidelinesInheritance {

    namespace GuidelinesCoreCpp_ClassVsStructInvariants {

        // =======================================================================
        // Class Hierarchy

        class DrawableUIElement {
        public:
            virtual ~DrawableUIElement() {}
            virtual void render() const = 0;
            // ...
        };

        class AbstractButton : public DrawableUIElement {
        public:
            virtual void onClick() = 0;
            // ...
        };

        class PushButton : public AbstractButton {
            virtual void render() const override {};
            virtual void onClick() override {};
            // ...
        };

        class Checkbox : public AbstractButton {
            // ...
        };

        // =======================================================================
        // Question: Is this class inherently hierarchical?

        template<typename T>
        class Container {
        public:
            // list operations:
            virtual T& get() = 0;
            virtual void put(T&) = 0;
            virtual void insert(size_t position) = 0;
            // ...
            
            // vector operations:
            virtual T& operator[](int) = 0;
            virtual void sort() = 0;
            // ...
            
            // tree operations:
            virtual void balance() = 0;
            // ...
        };

        static void guidelines_inherently_hierarchical() {

            Container<int>* container = nullptr;
        }
    }

    namespace GuidelinesInheritance_Comparison 
    {
        // =======================================================================
        // Comparison virtual / non virtual methods

        class A
        {
        public:
            void func() {};
        };

        class Base
        {
        public:
            virtual void func() {};
        };

        class Derived : public Base
        {
        public:
            virtual void func() override {};
        };

        class X
        {
        private:
            double m_value{};

        public:
            void func() {};
        };

        class Y
        {
        private:
            double m_value{};

        public:
            virtual void func() {};
        };

        static void guidelines_inheritance_virtual()
        {
            // comparison virtual / non-virtual method invocation
            A a;
            a.func();

            Base* bp;
            bp = new Derived();
            bp->func();

            // comparison object sizes of classes with / without virtual methods
            X x;
            Y y;

            std::println("Sizeof x: {}", sizeof(x));
            std::println("Sizeof y: {}", sizeof(y));
        }
    }

    namespace GuidelinesInheritance_Keyword_Override
    {
        // =======================================================================
        // Keyword 'override'

        class Base
        {
        public:
            // user wants to override this method in the derived class
            virtual void func() { std::println("In base class"); }
        };

        class Derived : public Base
        {
        public:
            // did a silly mistake by putting an argument "int a"
            void func(int a) /*override*/   // <=== remove comment
            {
                std::println("In derived class");
            }
        };

        static void guidelines_inheritance_keyword_override()
        {
            Base base;
            Derived derived;
            std::println("Compiles successfully");

            Base* bp;
            bp = &derived;
            bp->func();
        }
    }

    namespace GuidelinesInheritance_Keyword_Final_01
    {
        // =======================================================================
        // Keyword 'final' // 1. Example

        struct Base {
            virtual void func() {};
        };

        struct Derived : public Base {
            void func() final {};                   // virtual as it overrides base::f
        };

        struct MostDerived : public Derived {
            // void func() {};                      // error: cannot override!
        };
    }

    namespace GuidelinesInheritance_Keyword_Final_02
    {
        // =======================================================================
        // Keyword 'final' // 2. Example

        struct Base /*final*/ {                     // <=== remove comment
            virtual void func() {};
        };

        struct Derived : public Base {
        };
    }

    namespace GuidelinesInheritance_Keyword_Final_03
    {
        // =======================================================================
        // Keyword 'final' // 3. Example

        class IAbstract
        {
        public:
            virtual void doSomething() = 0;
        };

        class CDerived : public IAbstract
        {
        private:
            int m_x{};

        public:
            void doSomething() final { m_x = 1; }

            void anotherMethod (void) { doSomething(); }
        };
    }

    namespace GuidelinesInheritance_Virtual_BaseClassDestructor
    {
        // =======================================================================
        // Virtual Base Class Destructor

        class Base
        {
        public:
            /*virtual*/ ~Base() {
                std::println("d'tor Base");
            }

            virtual void doSomething() {}
        };

        class Derived : public Base
        {
        public:
            ~Derived() {
                std::println("d'tor Derived");
            }
        };

        static void test_base_class_destructor() {
            Base* bp = new Derived();
            delete bp;
        }
    }
}

void clean_code_guidelines_inheritance()
{
    using namespace GuidelinesInheritance;

    GuidelinesInheritance_Comparison::guidelines_inheritance_virtual();
    GuidelinesInheritance_Keyword_Override::guidelines_inheritance_keyword_override();
    GuidelinesInheritance_Virtual_BaseClassDestructor::test_base_class_destructor();
}

// ===========================================================================
// End-of-File
// ===========================================================================
