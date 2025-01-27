// ===========================================================================
// MemoryManagement.cpp
// ===========================================================================

#include <bitset>
#include <iostream>
#include <memory>
#include <print>
#include <type_traits>
#include <cassert>

namespace MemoryManagement {

    // =======================================================================
    // Examine Stack bevaviour

    static void func1()
    {
        auto i{ 1 };
        std::cout << "func1(): " << std::addressof(i) << " - i: " << i << '\n';
    }

    static void func2()
    {
        auto i{ 2 };
        std::cout << "func2(): " << std::addressof(i) << " - i: " << i << '\n';
        func1();
    }

    static void test_examine_stack_bevaviour() {

        auto i{ 0 };
        std::cout << "main():  " << std::addressof(i) << " - i: " << i << '\n';
        func2();
        func1();
    }

    // =======================================================================
    // Examine Stack Size

    static void func (const std::byte* stack_bottom_addr) {
        std::byte data[1024] = {};
        std::cout << stack_bottom_addr - data << '\n';

        func(stack_bottom_addr);
    }

    static void test_examine_stack_size() {

        std::byte b{};
        func(&b);
    }
}

// =======================================================================
// Memory alignment

namespace MemoryManagement {

    static void printBinary(std::string sv, void* p) {

        uint64_t tmp = reinterpret_cast<uint64_t>(p);
        std::bitset<64> bset(tmp);
        std::cout << sv << ':' << bset << std::endl;
    }

    static void test_examine_alignment_001() {

        size_t align_of_char      { std::alignment_of<char>::value      };
        size_t align_of_short     { std::alignment_of<short>::value     };
        size_t align_of_int       { std::alignment_of<int>::value       };
        size_t align_of_long      { std::alignment_of<long>::value      } ;
        size_t align_of_long_long { std::alignment_of<long long>::value };
        size_t align_of_float     { std::alignment_of<float>::value     };
        size_t align_of_double    { std::alignment_of<double>::value    };

        std::println("std::alignment_of<char>::value       {}", align_of_char    );
        std::println("std::alignment_of<short>::value      {}", align_of_short);
        std::println("std::alignment_of<int>::value        {}", align_of_int);
        std::println("std::alignment_of<long>::value       {}", align_of_long);
        std::println("std::alignment_of<long long>::value  {}", align_of_long_long);
        std::println("std::alignment_of<float>::value      {}", align_of_float);
        std::println("std::alignment_of<double>::value     {}", align_of_double);
    }

    static void test_examine_alignment_002() {

        size_t align_of_char      { alignof (char)      };
        size_t align_of_short     { alignof (short)     };
        size_t align_of_int       { alignof (int)       };
        size_t align_of_long      { alignof (long)      };
        size_t align_of_long_long { alignof (long long) };
        size_t align_of_float     { alignof (float)     };
        size_t align_of_double    { alignof (double)    };

        std::println("alignof (char)      {}", align_of_char);
        std::println("alignof (short)     {}", align_of_short);
        std::println("alignof (int)       {}", align_of_int);
        std::println("alignof (long)      {}", align_of_long);
        std::println("alignof (long long) {}", align_of_long_long);
        std::println("alignof (float)     {}", align_of_float);
        std::println("alignof (double)    {}", align_of_double);
    }

    struct Empty{};

    struct Struct
    {
        std::int8_t  m_int8;
        std::int16_t m_int16;
    };

    static void test_examine_alignment_003() {

        size_t align_of_empty { alignof (Empty) };
        size_t align_of_struct { alignof (Struct) };

        std::println("alignof (struct Empty)  {}", align_of_empty);
        std::println("alignof (struct Struct) {}", align_of_struct);
    }

    static void test_examine_alignment_004() {

        char bad{};
        
        *(char volatile*) &bad; // inject an anoying offset
        
        std::byte place1[sizeof(int64_t)]; // not properly aligned for int64_t
        
        int64_t *ptr1 = new (place1) int64_t(42); // Undefined Behaviour
    }

    static void test_examine_alignment_005()
    {
        alignas(16) int a[4] = {};
        alignas(1024) int b[4] = {};

        std::cout << &a << std::endl;
        std::cout << &b << std::endl;

        printBinary("a", &a);
        printBinary("b", &b);
    }


    static void test_examine_alignment_006()
    {
        // Forcing alignment using 'alignas'

        alignas(size_t) std::byte buffer[sizeof(size_t)] = {};

        size_t* ptr {new (buffer) size_t{ 123 } };    // Ok 

        *ptr = 71'777'214'294'589'695;                // Ok - for better readability with the debugger

        ptr->~size_t();
    }

    static void test_examine_alignment_007()
    {
        // ==============================================================
        // std::align provides a runtime solution for aligning a variable

        std::byte buffer[32] = {};

        std::cout << "Buffer:               " << &buffer << std::endl;
        std::cout << "Buffer Size:          " << sizeof(buffer) << std::endl;

        size_t alignof_int64_t{ alignof (int64_t) };    // requested alignment
        size_t sizeof_int64_t{ sizeof(int64_t) };       // requested storage size

        void* ptr{ buffer + 2 };     // not properly aligned for int64_t
        size_t size{ 32 - 2 };       // remaining space in place3

        std::cout << "Modified Buffer:      " << ptr << std::endl;

        size_t oldSize{ size };      // want to compare old an new size
        void* oldPtr{ ptr };         // want to compare old an new pointer

        void* p = std::align(
            alignof_int64_t,         // desired alignment
            sizeof_int64_t,          // size of the storage to be aligned
            ptr,                     // pointer to contiguous storage(a buffer) in which to operate
            size                     // buffer size
        );

        std::cout << "Aligned Buffer:       " << ptr << std::endl;
        std::cout << "Size of this Buffer:  " << size << std::endl;

        assert(p == ptr);

        auto diff = reinterpret_cast<uint8_t*>(ptr) - reinterpret_cast<uint8_t*>(oldPtr);

        std::cout << "Bytes of new buffer: " << diff << std::endl;
    }

    // Vom Buch - etrwas unklar
    static bool is_aligned(void* ptr, size_t alignment) {

        if (ptr == nullptr) {  // make sure that the ptr argument isn't null
            return false;
        }

        if (! std::has_single_bit(alignment)) {   // expecting power of 2
            return false;
        }

        auto s = std::numeric_limits<std::size_t>::max();

        auto aligned_ptr = ptr;

        std::align (
            alignment,    // the desired alignment
            1,            // the size of the storage to be aligned
            aligned_ptr,  // pointer to contiguous storage (a buffer) of at least space bytes
            s             // the size of the buffer in which to operate
        );

        return ptr == aligned_ptr;
    }

    static void test_examine_alignment_008()
    {


        alignas(std::max_align_t) char buffer[512] = {};  // typical use
    }

    static void test_examine_alignment_02() {

        auto* ptr{ new int{} };
        bool b{ is_aligned(ptr, 4) };   // true
        delete ptr;
    }

    static void test_examine_alignment_03() {

        auto* ptr{ new int{} };
        auto* copy{ ptr };

        // add offset of 1 byte to this address
        ptr = reinterpret_cast<int*>((reinterpret_cast<char*> (ptr) + 1));

        bool b{ is_aligned(ptr, 4) };   // false
        delete copy;  
    }

}

// =================================================================

void memory_management()
{
    using namespace MemoryManagement;

    //test_examine_alignment_001();
    //test_examine_alignment_002();
  //  test_examine_alignment_003();
   // test_examine_alignment_004();

   // test_examine_alignment_005();

    //test_examine_alignment_006();


  //  test_examine_alignment_007();

    test_examine_alignment_008();

   // test_examine_stack_bevaviour();
    // test_examine_stack_size();        // crashes (intentionally)

    //test_examine_alignment_01();
    //test_examine_alignment_02();
   // test_examine_alignment_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
