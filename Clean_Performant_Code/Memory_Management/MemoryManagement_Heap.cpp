// ===========================================================================
// MemoryManagement_Heap.cpp // Memory Management
// ===========================================================================

#include <cstdlib>
#include <new>
#include <print>

namespace MemoryManagement {


    namespace Using_New_delete {

        // =======================================================================

        class Foo
        {
        public:
            Foo() {}
            Foo(int x) {}
        };

        static void test_01_single_object() {

            // allocating and deallocating a single variable or object

            int* p = new int(123);   // dynamically allocate one int
            delete p;                // deallocate it

            Foo* f = new Foo{ 123 }; // same for class type
            delete f;
        }

        // =======================================================================

        static void test_02_arrays() {

            // allocating and deallocating arrays of variables or objects

            int* arr = new int[5];     // allocate array of 5 ints
            delete[] arr;              // must use delete[]

            // delete arr;             // WRONG — must use delete[] - UB (undefined behaviour)

            int* arr2 = new int[5] {}; // with initialization
            delete[] arr2;

            Foo* foos1 = new Foo[3];   // allocate an array of objects
            delete[] foos1;

            Foo* foos2 = new Foo[3]{ {1}, {2}, {3} };  // with initialization
            delete[] foos2;
        }

        // https://en.cppreference.com/w/cpp/memory/new/nothrow.html

        // =======================================================================

        static void test_03_nothrow_new_01() {

            int* p = new (std::nothrow) int;

            if (!p) {
                // allocation failed
            }

            delete p;
        }

        static void test_03_nothrow_new_02() {

            constexpr unsigned long long Size = 1'000'000'000ul;

            try
            {
                while (true)
                {
                    new int[Size];   // throwing overload
                    std::println("Allocation new int succeeded.");
                }
            }
            catch (const std::bad_alloc& e)
            {
                std::println("Exception: {}", e.what());
            }

            while (true)
            {
                int* p = new(std::nothrow) int[Size]; // non-throwing overload
                if (p == nullptr)
                {
                    std::println("Allocation returned nullptr");
                    break;
                }
                else {
                    std::println("Allocation new(std::nothrow) succeeded.");
                }
            }
        }

        static void test_03_nothrow_new() {

            test_03_nothrow_new_01();
            test_03_nothrow_new_02();
        }

        // =======================================================================

        static void test_04_value_initialization_vs_default_initialization() {

            // Default-Initialization
            int* p1 = new int;         // uninitialized value
            delete p1;

            // Value-initialization
            int* p2 = new int{};       // initialized to 0
            delete p2;

            // Default-Initialization
            Foo* f1 = new Foo;         // default constructor
            delete f1;

            // Value-initialization
            Foo* f2 = new Foo{};       // also default constructor
            delete f2;
        }

        // =======================================================================

        // class-specific allocation

        class AnotherFoo
        {
        public:
            static void* operator new(std::size_t size) {
                std::println("operator new: >>> {} bytes");
                return std::malloc(size);
            }

            static void operator delete(void* ptr) {
                std::println("operator delete: >>> {} ptr");
                std::free(ptr);
            }
        };

        static void test_05_custom_new_custom_delete() {

            AnotherFoo* f = new AnotherFoo;
            delete f;

        }

    }
}

void memory_management_heap()
{
    using namespace MemoryManagement;

    Using_New_delete::test_01_single_object();
    Using_New_delete::test_02_arrays();
    // Using_New_delete::test_03_nothrow_new();
    Using_New_delete::test_04_value_initialization_vs_default_initialization();
}

// ===========================================================================
// End-of-File
// ===========================================================================
