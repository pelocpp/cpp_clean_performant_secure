// ===========================================================================
// MemoryManagement_Heap.cpp // Memory Management
// ===========================================================================

#include <cstdlib>
#include <new>
#include <print>

namespace MemoryManagement {

    namespace Using_New_delete {

        // ===================================================================
        // Allocating and deallocating a single variable or object

        class Foo
        {
        public:
            Foo() {}
            Foo(int x) {}
        };

        static void test_01_single_object() {

            int* p = new int(123);   // dynamically allocate one int
            delete p;                // deallocate it

            Foo* f = new Foo{ 123 }; // same for class type
            delete f;
        }

        // ===================================================================
        // Allocating and deallocating arrays of variables or objects

        static void test_02_arrays() {

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

        // ===================================================================
        // nothrow new (Non-Throwing Allocation)
        // https://en.cppreference.com/w/cpp/memory/new/nothrow.html

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

        // ===================================================================
        // Value-Initialization vs Default-Initialization

        static void test_04_value_initialization_vs_default_initialization() {

            // default-Initialization
            int* p1 = new int;         // uninitialized value
            delete p1;

            // value-initialization
            int* p2 = new int{};       // initialized to 0
            delete p2;

            // default-Initialization
            Foo* f1 = new Foo;         // default constructor
            delete f1;

            // value-initialization
            Foo* f2 = new Foo{};       // also default constructor
            delete f2;
        }

        // ===================================================================
        // Class-specific allocation

        class AnotherFoo
        {
        public:
            static void* operator new(std::size_t size)
            {
                void* ptr = std::malloc(size);
                std::println("operator new:    >>> {:#X} - {} bytes", reinterpret_cast<intptr_t>(ptr), size);
                return ptr;
            }

            static void operator delete(void* ptr)
            {
                std::println("operator delete: >>> {:#X}", reinterpret_cast<intptr_t>(ptr));
                std::free(ptr);
            }
        };

        static void test_05_custom_new_custom_delete() {

            AnotherFoo* f = new AnotherFoo;
            delete f;
        }


        // ===================================================================
        // Aligned Allocation (C++17)

        //static void test_09()
        //{
        //    int n = 123;
        //    int* ip = &n;

        //    std::println("&n: {:#X}", reinterpret_cast<intptr_t>(ip));
        //    std::println("&n: {:#x}", reinterpret_cast<intptr_t>(&n));
        //}

        struct alignas(64) Big {
            int x;
        };

        // https://stackoverflow.com/questions/56753138/how-to-know-if-the-address-is-64-bit-aligned

        // Wie kann man überprüfen, ob eine Adresse 64 Bit ausgerichtet ist !!!

        static void test_06_aligned_allocation() {

            Big* ptr = new Big;

            std::println("ptr: {:#X}", reinterpret_cast<intptr_t>(ptr));

            delete ptr;
        }
    }
}

void memory_management_heap()
{
    using namespace MemoryManagement;

    //Using_New_delete::test_01_single_object();
    //Using_New_delete::test_02_arrays();
    //// Using_New_delete::test_03_nothrow_new();    // runs very long
    //Using_New_delete::test_04_value_initialization_vs_default_initialization();
    //Using_New_delete::test_05_custom_new_custom_delete();
    Using_New_delete::test_06_aligned_allocation();
}

// ===========================================================================
// End-of-File
// ===========================================================================
