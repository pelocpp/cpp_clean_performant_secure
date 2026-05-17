#include <print>

void test_02_alloc_dealloc_mismatch()
{
    std::println("Allocate / Deallocate Mismatch:");

    int ready{ 1 };
    int* adr{ nullptr };

    while (ready != 0) {

        std::println("Choose action:");
        std::println("(1) = Allocate memory with new");
        std::println("(2) = Allocate memory with new[]");
        std::println("(3) = Deallocate memory with delete");
        std::println("(4) = Deallocate memory with delete[]");
        std::println("(9) = Exit");
        std::println();

        int action { - 1 };

        scanf_s("%d", &action);

        switch (action)
        {
        case 1:
            adr = new int{ 123 };
            break;
        case 2:
            adr = new int [20] {};
            break;
        case 3:
            delete adr;
            break;
        case 4:
            delete[] adr;
            break;
        case 9:
            ready = 0;
            break;
        default:
            printf("Unknown command: %d", action);
            break;
        }
    }
}
