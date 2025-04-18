// ===========================================================================
// MemoryManagement.cpp
// ===========================================================================

#include <bitset>
#include <cassert>
#include <iostream>
#include <memory>
#include <print>
#include <string>
#include <type_traits>

namespace MemoryManagement {

    namespace Stack_Debug_Versus_Release_Mode {

        // =============================================================== 
        // Examine Stack Layout: Debug versus Release Mode

        static void test_stack_debug_versus_release_mode()
        {
            int a = 1;
            int b = 2;
            int c = 3;
            int d = 4;

            int* ap = &a;
            int* bp = &b;
            int* cp = &c;
            int* dp = &d;

            unsigned long long d1 = (unsigned long long)bp - (unsigned long long)ap;
            unsigned long long d2 = (unsigned long long)cp - (unsigned long long)bp;
            unsigned long long d3 = (unsigned long long)dp - (unsigned long long)cp;

            std::cout << "&a:  " << &a << std::endl;
            std::cout << "&b:  " << &b << std::endl;
            std::cout << "&c:  " << &c << std::endl;
            std::cout << "&d:  " << &d << std::endl;

            std::println("Diff a <=> b: {}", d1);
            std::println("Diff b <=> c: {}", d2);
            std::println("Diff c <=> d: {}", d3);
        }
    }

    namespace Stack_Behaviour {

        // =======================================================================
        // Examine Stack behaviour

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

        static void test_examine_stack_behaviour() {

            auto i{ 0 };
            std::cout << "main():  " << std::addressof(i) << " - i: " << i << '\n';
            func2();
            func1();
        }
    }

    namespace Stack_Size {

        // =======================================================================
        // Examine Stack Size

        static void func(const std::byte* stack_bottom_addr) {
            std::byte data[1024] = {};
            std::cout << stack_bottom_addr - data << '\n';

            func(stack_bottom_addr);
        }

        static void test_examine_stack_size() {

            std::byte b{};
            func(&b);
        }
    }

    namespace MemoryAlignment {

        // =======================================================================
        // Memory Alignment
        
        static void printBinary(std::string sv, void* p) {

            uint64_t tmp = reinterpret_cast<uint64_t>(p);
            std::bitset<64> bset(tmp);
            std::cout << sv << ": " << bset << std::endl;
        }

        static void test_examine_alignment_01() {

            std::println("alignof (char)        {}", alignof (char));
            std::println("alignof (short)       {}", alignof (short));
            std::println("alignof (int)         {}", alignof (int));
            std::println("alignof (long)        {}", alignof (long));
            std::println("alignof (long long)   {}", alignof (long long));
            std::println("alignof (float)       {}", alignof (float));
            std::println("alignof (double)      {}", alignof (double));
            std::println("alignof (long double) {}", alignof (long double));
        }

        static void test_examine_alignment_02() {

            size_t align_of_char{ std::alignment_of<char>::value };
            size_t align_of_short{ std::alignment_of<short>::value };
            size_t align_of_int{ std::alignment_of<int>::value };
            size_t align_of_long{ std::alignment_of<long>::value };
            size_t align_of_long_long{ std::alignment_of<long long>::value };
            size_t align_of_float{ std::alignment_of<float>::value };
            size_t align_of_double{ std::alignment_of<double>::value };

            std::println("std::alignment_of<char>::value       {}", align_of_char);
            std::println("std::alignment_of<short>::value      {}", align_of_short);
            std::println("std::alignment_of<int>::value        {}", align_of_int);
            std::println("std::alignment_of<long>::value       {}", align_of_long);
            std::println("std::alignment_of<long long>::value  {}", align_of_long_long);
            std::println("std::alignment_of<float>::value      {}", align_of_float);
            std::println("std::alignment_of<double>::value     {}", align_of_double);
        }

        struct Empty {};

        struct Struct
        {
            std::int8_t  m_int8;
            std::int16_t m_int16;
        };

        static void test_examine_alignment_03() {

            size_t align_of_empty{ alignof (Empty) };
            size_t align_of_struct{ alignof (Struct) };

            std::println("alignof (struct Empty)  {}", align_of_empty);
            std::println("alignof (struct Struct) {}", align_of_struct);
        }

        static void test_examine_alignment_04() {

            char bad{};

            *(char volatile*)&bad; // inject an bad offset

            std::byte buffer[sizeof(int64_t)] = {};     // not properly aligned for int64_t

            int64_t* ptr1 = new (buffer) int64_t(42);   // undefined behaviour
        }

        static void test_examine_alignment_05()
        {
            alignas(16) int a{};
            alignas(1024) int b{};

            std::cout << &a << std::endl;
            std::cout << &b << std::endl;

            printBinary("a", &a);
            printBinary("b", &b);
        }


        static void test_examine_alignment_06()
        {
            // Forcing alignment using 'alignas'

            alignas(size_t) std::byte buffer[sizeof(size_t)] = {};

            size_t* ptr{ new (buffer) size_t{ 123 } };    // Ok 

            *ptr = 71'777'214'294'589'695;                // Ok - for better readability with the debugger

            ptr->~size_t();
        }

        static void test_examine_alignment_07()
        {
            // ==============================================================
            // std::align provides a runtime solution for aligning a variable

            std::byte buffer[32] = {};

            std::cout << "Buffer Address:                 " << buffer << std::endl;
            std::cout << "Buffer Size:                    " << sizeof(buffer) << std::endl;

            size_t alignof_int64_t{ alignof(int64_t) };     // requested alignment
            size_t sizeof_int64_t{ sizeof(int64_t) };       // requested storage size

            void* ptr{ buffer + 2 };     // not properly aligned for int64_t
            size_t size{ 32 - 2 };       // remaining space in place3

            std::cout << "Modified Buffer Address:        " << ptr << std::endl;

            size_t oldSize{ size };      // want to compare old size with new calculated size
            void* oldPtr{ ptr };         // want to compare old ptr with new calculated pointer

            void* p = std::align(
                alignof_int64_t,         // desired alignment
                sizeof_int64_t,          // size of the storage to be aligned
                ptr,                     // pointer to contiguous storage(a buffer) in which to operate
                size                     // buffer size
            );

            std::cout << "Aligned Buffer Address:         " << ptr << std::endl;
            std::cout << "Size of new Buffer :            " << size << std::endl;

            assert(p == ptr);

            auto diff = reinterpret_cast<uint8_t*>(ptr) - reinterpret_cast<uint8_t*>(oldPtr);

            std::cout << "Number of skipped Bytes:        " << diff << std::endl;
        }

        // Vom Buch - etwas unklar
        static bool is_aligned(void* ptr, size_t alignment) {

            if (ptr == nullptr) {  // make sure that the ptr argument isn't null
                return false;
            }

            if (!std::has_single_bit(alignment)) {   // expecting power of 2
                return false;
            }

            auto s = std::numeric_limits<std::size_t>::max();

            auto aligned_ptr = ptr;

            std::align(
                alignment,    // the desired alignment
                1,            // the size of the storage to be aligned
                aligned_ptr,  // pointer to contiguous storage (a buffer) of at least space bytes
                s             // the size of the buffer in which to operate
            );

            return ptr == aligned_ptr;
        }

        static void test_examine_alignment_08()
        {
            alignas(std::max_align_t) char buffer[512] = {};  // typical use
            printBinary("a", buffer);
        }

        static void test_examine_alignment_09() {

            auto* ptr{ new int{} };
            bool b{ is_aligned(ptr, 4) };   // true
            delete ptr;
        }

        static void test_examine_alignment_10() {

            auto* ptr{ new int{} };
            auto* copy{ ptr };

            // add offset of 1 byte to this address
            ptr = reinterpret_cast<int*>((reinterpret_cast<char*>(ptr) + 1));

            bool b{ is_aligned(ptr, 4) };   // false
            delete copy;
        }
    }

    namespace MemoryPadding {

        // =======================================================================
        // Memory Padding

        class Document
        {
            bool       m_isCached;
            double     m_rank;
            int        m_id;
        };

        class Document_Behind_the_Scenes {
            bool       m_isCached;
            std::byte  m_padding1[7]; // invisible padding inserted by compiler
            double     m_rank;
            int        m_id;
            std::byte  m_padding2[4]; // invisible padding inserted by compiler
        };

        class Document_V2
        {
            double     m_rank;
            int        m_id;
            bool       m_isCached;
        };

        class Document_V2_Behind_the_Scenes {
            double     m_rank;
            int        m_id;
            bool       m_isCached;
            std::byte  m_padding1[3]; // invisible padding inserted by compiler
        };

        static void test_memory_padding() {

            std::println("sizeof(Document): {}", sizeof(Document));
            std::println("sizeof(Document_Behind_the_Scenes): {}", sizeof(Document_Behind_the_Scenes));

            std::println("sizeof(Document_V2): {}", sizeof(Document_V2));
            std::println("sizeof(Document_V2_Behind_the_Scenes): {}", sizeof(Document_V2_Behind_the_Scenes));
        }

    }

    namespace PlacementNew {

    // =======================================================================
    // Placement New

        class Person
        {
        private:
            std::string m_first;
            std::string m_last;
            size_t      m_age;

        public:
            Person(std::string first, std::string last, size_t age)
                : m_first{ first }, m_last{ last }, m_age{ age }
            {
                std::println("c'tor Person");
            }

            ~Person() {
                std::println("d'tor Person");
            }
        };

        static void test_placement_new_01() {

            auto* memory = std::malloc(sizeof(Person));
            auto* person = ::new (memory) Person{ "Sepp", "Mueller", (size_t)30 };

            person->~Person();
            std::free(memory);
        }

        static void test_placement_new_02() {

            auto* memory = std::malloc(sizeof(Person));
            auto* person = reinterpret_cast<Person*>(memory);

            std::uninitialized_fill_n(person, 1, Person{ "Sepp", "Mueller", (size_t)30 });

            std::destroy_at(person);
            std::free(memory);
        }

        static void test_placement_new_03() {

            auto* memory = std::malloc(sizeof(Person));
            auto* person = reinterpret_cast<Person*>(memory);

            std::construct_at(person, Person{ "Sepp", "Mueller", (size_t)30 }); // C++20

            std::destroy_at(person);
            std::free(memory);
        }
    }

    namespace MemoryLaundry {

        using namespace PlacementNew;  // class Person

        // =======================================================================
        // Memory Laundry: std::launder
        static void test_std_launder()
        {
            constexpr int CountPersons{ 5 };

            alignas(class Person) unsigned char buffer[sizeof(class Person) * CountPersons];

            for (size_t i{}; i != CountPersons; ++i) {

                std::string first{ "first_name_" };
                std::string last{ "last_name_" };

                // manually construct objects using placement new
                new(buffer + sizeof(Person) * i) Person{ first + std::to_string(i), last + std::to_string(i), i };
            }

            auto ptr{ std::launder(reinterpret_cast<Person*>(buffer)) };

            for (size_t i{}; i != CountPersons; ++i) {

                std::destroy_at(&ptr[i]);
            }
        }
    }
}

// ===========================================================================

void memory_management()
{
    using namespace MemoryManagement;

    //Stack_Debug_Versus_Release_Mode::test_stack_debug_versus_release_mode();

    //Stack_Behaviour::test_examine_stack_behaviour();
    ////Stack_Size::test_examine_stack_size();               // crashes (intentionally)

    MemoryAlignment::test_examine_alignment_01();
    //MemoryAlignment::test_examine_alignment_02();
    //MemoryAlignment::test_examine_alignment_03();
    //MemoryAlignment::test_examine_alignment_04();
    //MemoryAlignment::test_examine_alignment_05();
    //MemoryAlignment::test_examine_alignment_06();
    //MemoryAlignment::test_examine_alignment_07();
    //MemoryAlignment::test_examine_alignment_08();
    //MemoryAlignment::test_examine_alignment_09();
    //MemoryAlignment::test_examine_alignment_10();

    //MemoryPadding::test_memory_padding();

    //PlacementNew::test_placement_new_01();
    //PlacementNew::test_placement_new_02();
    //PlacementNew::test_placement_new_03();

    //MemoryLaundry::test_std_launder();
}

// ===========================================================================
// End-of-File
// ===========================================================================
