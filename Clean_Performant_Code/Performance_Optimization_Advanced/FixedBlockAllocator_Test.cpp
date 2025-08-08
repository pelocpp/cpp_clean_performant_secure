// ===========================================================================
// FixedBlockAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "FixedBlockAllocator.h"

#include <forward_list>
#include <print>

#include <complex>

// static const size_t ArenaSize = 100;
static const size_t ArenaSize = 500;
static alignas(std::max_align_t) char arena[ArenaSize]{};

FixedBlockMemoryManager<FixedArenaController> memoryManager{ arena };

namespace FixedBlockAllocatorTest {

    static void test_fixed_block_allocator_01()
    {
        FixedBlockAllocator<double> alloc;

        std::forward_list<double, FixedBlockAllocator<double>> list{ alloc };

        list.push_front(1.5);
        list.push_front(2.5);
        list.push_front(3.5);
        list.push_front(4.5);
        list.push_front(5.5);

        for (auto elem : list) {
            std::println("{} ", elem);
        }
    }

    static void test_fixed_block_allocator_02()
    {
        FixedBlockAllocator<int> alloc;
        std::forward_list<int, FixedBlockAllocator<int>> list{ alloc };

        list.push_front(1);
        list.push_front(2);
        list.push_front(3);

        for (auto elem : list) {
            std::println("{} ", elem);
        }
    }

    static void test_fixed_block_allocator_03()
    {
        FixedBlockAllocator<std::string> alloc;
        std::forward_list<std::string, FixedBlockAllocator<std::string>> list{ alloc };

        std::println("sizeof (std::string): {} ", sizeof (std::string));
        std::println("sizeof (std::complex<double>): {} ", sizeof(std::complex<double>));
        std::println("sizeof (std::complex<float>): {} ", sizeof(std::complex<float>));

        list.push_front(std::string{ "ABC"});
        list.push_front(std::string{ "DEF" });
        list.push_front(std::string{ "GHI" });

        for (const auto& elem : list) {
            std::println("{} ", elem);
        }
    }
}

void test_fixed_block_allocator()
{
    using namespace FixedBlockAllocatorTest;

    test_fixed_block_allocator_01();
    test_fixed_block_allocator_02();
    test_fixed_block_allocator_03();
}

// ===========================================================================
// End-of-File
// ===========================================================================
