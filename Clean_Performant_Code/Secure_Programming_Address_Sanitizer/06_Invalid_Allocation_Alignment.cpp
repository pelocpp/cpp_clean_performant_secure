// ===========================================================================
// 06_Invalid_Allocation_Alignment.cpp // Visual Studio Address Sanitizer
// ===========================================================================

#include <Windows.h>

int ExternalAlign = 5;

void test_06_invalid_allocation_alignment() {

    // this externally calculated alignment of 5 isn't valid.

    void* ptr = _aligned_malloc(8, ExternalAlign);

    bool result = (ptr == nullptr && errno == EINVAL) ? 0 : -1;
}

// ===========================================================================
// End-of-File
// ===========================================================================
