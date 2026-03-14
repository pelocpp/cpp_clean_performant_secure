// =====================================================================================
// PMR_03.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "PMR_DumpBuffer.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <exception>
#include <limits>
#include <list>
#include <memory_resource>
#include <print>
#include <set>
#include <span>
#include <string_view>
#include <vector>

// =====================================================================================
// First example, using a polymorphic allocator object

static constexpr std::size_t NumBytes = 176;

static std::array<std::uint8_t, NumBytes> g_memory{};

static void test_pmr_03_01()
{
    // needing buffer size 16

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource pool{ g_memory.data(), g_memory.size() };

    std::pmr::polymorphic_allocator<std::int32_t> allocator(&pool);

    std::int32_t* ptr{ nullptr };

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'A';

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'B';

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'C';

    std::span span{ g_memory };
    dumpBuffer(span);
}

// =====================================================================================
// Second example, using a polymorphic allocator object, 
// using std::uint8_t variables

static std::array<std::uint8_t, NumBytes> g_smallBuffer{};

static void test_pmr_03_02()
{
    // needing buffer size 16

    std::fill(
        g_smallBuffer.begin(),
        g_smallBuffer.end(),
        std::numeric_limits<std::uint8_t>::max()
    );

    std::pmr::monotonic_buffer_resource pool{
        g_smallBuffer.data(),
        g_smallBuffer.size(),
        std::pmr::null_memory_resource()
    };

    std::pmr::polymorphic_allocator<std::uint8_t> allocator(&pool);

    std::uint8_t* ptr{ nullptr };

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'A';

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'B';

    ptr = allocator.allocate(1);
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 'C';

    dumpBuffer(std::span{ g_smallBuffer });
}

// =====================================================================================

void test_pmr_03()
{
    test_pmr_03_01();
    test_pmr_03_02();
}

// =====================================================================================
// End-of-File
// =====================================================================================
