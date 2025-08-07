// ===========================================================================
// FixedArenaController_Test.cpp //  // Performance Optimization Advanced
// ===========================================================================

#include "FixedArenaController.h"

namespace FixedArenaController_Test {

    const int ArenaLength = 40;
    static alignas(std::max_align_t) char arena[ArenaLength];

    static void test_fixed_arena_controller_01()
    {
        FixedArenaController controller{ arena };

        controller.dump();

        int* ptr = static_cast<int*> (controller.allocate(sizeof(int)));
        *ptr = 123;

        controller.dump();
    }

    static void test_fixed_arena_controller_02()
    {
        FixedArenaController controller{ arena };

        int* ptr = static_cast<int*> (controller.allocate(sizeof(int)));
        *ptr = 123;
    }
}

void test_fixed_arena_controller()
{
    FixedArenaController_Test::test_fixed_arena_controller_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
