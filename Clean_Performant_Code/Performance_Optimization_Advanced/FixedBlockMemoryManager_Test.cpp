// ===========================================================================
// FixedBlockMemoryManager_Test.cpp //  // Performance Optimization Advanced
// ===========================================================================

#include "FixedArenaController.h"

#include "FixedBlockMemoryManager.h"

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

        int* p3 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        *p3 = 3;

        //int* p4 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        //*p4 = 4;

        //int* p5 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        //*p5 = 5;

        //int* p6 = static_cast<int*> (memoryManager.allocate(sizeof(int)));
        //*p6 = 6;
    }
}

void test_block_memory_manager()
{
    using namespace FixedBlockMemoryManagerTest;

    //test_fixed_block_memory_manager_01();
    test_fixed_block_memory_manager_02();
}

// ===========================================================================
// End-of-File
// ===========================================================================
