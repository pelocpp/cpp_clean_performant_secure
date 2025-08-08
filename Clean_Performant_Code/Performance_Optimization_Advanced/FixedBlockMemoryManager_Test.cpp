// ===========================================================================
// FixedBlockMemoryManager_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "FixedBlockMemoryManager.h"

#include <print>

namespace FixedBlockMemoryManagerTest {

    static void test_fixed_block_memory_manager_01()
    {
        const int ArenaLength = 40;
        static alignas(std::max_align_t) char arena[ArenaLength];

        FixedBlockMemoryManager<FixedArenaController> memoryManager{ arena };

        int* ptr = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *ptr = 123;

        memoryManager.deallocate(ptr);
    }

    static void test_fixed_block_memory_manager_02()
    {
        const int ArenaLength = 16;
        static alignas(std::max_align_t) char arena[ArenaLength];

        FixedBlockMemoryManager<FixedArenaController> memoryManager{ arena };

        int* p1 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *p1 = 1;

        int* p2 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *p2 = 2;
        
        try
        {
            int* p3 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        }
        catch (std::bad_alloc&) {
            std::println("Out of memory!");
        }
    }

    static void test_fixed_block_memory_manager_03()
    {
        const int ArenaLength = 16;
        static alignas(std::max_align_t) char arena[ArenaLength];

        FixedBlockMemoryManager<FixedArenaController> memoryManager{ arena };

        int* p1 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *p1 = 1;

        int* p2 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *p2 = 2;

        memoryManager.deallocate(p1);
        memoryManager.deallocate(p2);
    }

    static void test_fixed_block_memory_manager_04()
    {
    }
}

void test_block_memory_manager()
{
    using namespace FixedBlockMemoryManagerTest;

    //test_fixed_block_memory_manager_01();
    //test_fixed_block_memory_manager_02();
    //test_fixed_block_memory_manager_03();
    test_fixed_block_memory_manager_04();
}

// ===========================================================================
// End-of-File
// ===========================================================================
