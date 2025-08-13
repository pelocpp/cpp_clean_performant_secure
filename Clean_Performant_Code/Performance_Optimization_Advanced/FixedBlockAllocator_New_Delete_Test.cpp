// ===========================================================================
// FixedBlockAllocator_New_Delete_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include "FixedBlockAllocator.h"

#include <print>

static const size_t ArenaSize = 8'000;    // needed for new / delete examples
static alignas(std::max_align_t) char arena[ArenaSize]{};
static FixedBlockMemoryManager<FixedArenaController> memoryManager{ arena };

namespace FixedBlockAllocatorTest_NewDelete {

    // class with class-specific new and delete operator
    class MemoryManagerTester
    {
    private:
        size_t m_content;

    public:
        MemoryManagerTester(size_t content)
            : m_content{ content }
        {
        }

        static void* operator new(size_t s)
        {
            return memoryManager.allocate(s);
        }

        static void  operator delete(void* p)
        {
            memoryManager.deallocate(p);
        }
    };

    static void test_fixed_block_allocator_01()
    {
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

    static void test_fixed_block_allocator_02()
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
}

// ===========================================================================

void test_fixed_block_allocator_new_delete()
{
    using namespace FixedBlockAllocatorTest_NewDelete;

    test_fixed_block_allocator_01();
    test_fixed_block_allocator_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
