// ===========================================================================
// MemoryManagement_Heap.cpp // Memory Management
// ===========================================================================

#include <new>
#include <print>

namespace MemoryManagement {

    namespace Using_New_delete {

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

            Foo* f = new Foo(123);
            delete f;
        }

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


        static void test_03_nothrow_new_01() {

            int* p = new (std::nothrow) int;

            if (!p) {
                // allocation failed
            }

            delete p;
        }

        static void test_03_nothrow_new_01() {

            try
            {
                while (true)
                {
                    new int[100000000ul];   // throwing overload
                }
            }
            catch (const std::bad_alloc& e)
            {
                std::cout << e.what() << '\n';
            }

            while (true)
            {
                int* p = new(std::nothrow) int[100000000ul]; // non-throwing overload
                if (p == nullptr)
                {
                    std::cout << "Allocation returned nullptr\n";
                    break;
                }
            }
        }



        static void test_03_nothrow_new() {
        }

    }
}


void memory_management_heap()
{
}

// ===========================================================================
// End-of-File
// ===========================================================================
