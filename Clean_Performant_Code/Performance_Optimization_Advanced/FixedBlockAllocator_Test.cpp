// ===========================================================================
// FixedBlockAllocator_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include "FixedBlockAllocator.h"

#include <forward_list>
#include <list>
#include <print>

//static const size_t ArenaSize = 200;
static const size_t ArenaSize = 8'000;    // needed for new / delete examples
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
        // works only if block size is adjusted to hold both list proxy objects
        // and list item objects, in this example 24 bytes.

        FixedBlockAllocator<int> alloc;
        std::list<int, FixedBlockAllocator<int>> list{ alloc };

        // Hmmmmmmmmmmmmmmmm:
        // Das interne Proxy-Objekt ist 16 Bytes groß,
        // die Knoten 24 Bytes
        // ===> Das geht dann bei der zweiten Allokation schief.

        list.push_back(1);
        list.push_back(2);
        list.push_back(3);

        for (int i : list) {
            std::println("{} ", i);
        }
    }
}

// ===========================================================================

// class with class-specific new and delete operator
class MemoryManagerTester
{
private:
    size_t m_contents;

public:
    MemoryManagerTester(size_t contents) 
        : m_contents{ contents } 
    {}

    static void* operator new(size_t s)
    { 
        return memoryManager.allocate(s); 
    }

    static void  operator delete(void* p)
    {
        memoryManager.deallocate(p);
    }
};

static void test_fixed_block_allocator_10()
{
    const size_t MaxIterations = 1000;
    {
        {
            std::println("MemoryManagerTester: class specific new");
            memoryManager.clear();
            new MemoryManagerTester(0);
        }

        {
            std::println("MemoryManagerTester: global new");
            ::new MemoryManagerTester(0);
        }
    }
}

static void test_fixed_block_allocator_11()
{
    const size_t MaxIterations = 10'000;
    {
        // using FixedBlockMemoryManager for 1000 size_t variables:
        // need at least an arena of 8 * 1000 bytes size.

        std::println("MemoryManagerTester: class specific new");
        ScopedTimer watch{};

        for (size_t i{}; i != MaxIterations; ++i) {
            memoryManager.clear();
            for (auto i = 0; i < 1000; ++i) {
                new MemoryManagerTester(i);
            }
        }
    }
    {
        std::println("MemoryManagerTester: global new");
        ScopedTimer watch{};

        for (size_t i{}; i != MaxIterations; ++i) {
            for (auto i = 0; i < 1000; ++i) {
                ::new MemoryManagerTester(i);
            }
        }
    }
}

void test_fixed_block_allocator()
{
    using namespace FixedBlockAllocatorTest;

    test_fixed_block_allocator_01();
    test_fixed_block_allocator_02();
    // test_fixed_block_allocator_03();   // may crash

    test_fixed_block_allocator_10();
    test_fixed_block_allocator_11();
}

// ===========================================================================
// End-of-File
// ===========================================================================
