// ===========================================================================
// Program.c // Small Safe Array Library
// ===========================================================================

extern void test_vec();
extern void test_int_vec();

extern void unit_test_vec();
extern void unit_test_int_vec();

int main()
{
    test_vec();
    test_int_vec();

    unit_test_vec();
    unit_test_int_vec();

    return 0;
}

// ===========================================================================
// End-of-File
// ===========================================================================
