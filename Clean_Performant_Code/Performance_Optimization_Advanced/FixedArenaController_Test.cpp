// ===========================================================================
// FixedArenaController_Test.cpp // Performance Optimization Advanced
// ===========================================================================

#include "FixedArenaController.h"

namespace FixedArenaControllerTest {

    const int ArenaLength = 40;
    static alignas(std::max_align_t) char arena[ArenaLength];

    static void test_fixed_arena_controller_01()
    {
        FixedArenaController controller{ arena };

        controller.dump();

        int* ptr = static_cast<int*> (controller.allocate(sizeof(int)));

        controller.dump();
    }
}

void test_fixed_arena_controller()
{
    FixedArenaControllerTest::test_fixed_arena_controller_01();
}

// ===========================================================================
// End-of-File
// ===========================================================================
