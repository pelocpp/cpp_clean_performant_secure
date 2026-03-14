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
// 1. Beispiel
// Eine monotonic_buffer_resource Resource mit Fallback

static constexpr std::size_t NumBytes = 176;

static std::array<std::uint8_t, NumBytes> g_memory{};

static void test_pmr_10_01()
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
// 2. Beispiel
// Wie vorher, nur einen Puffer mit std::uint8_t Werten:
// Die liegen im Trace dann schön nebeneinander

static std::array<std::uint8_t, NumBytes> g_smallBuffer{};

static void test_pmr_10_02()
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
// 3. Beispiel:
// Einen std::vector verwenden ....

static void test_pmr_10_03()
{
    // needing buffer size 32

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource pool{
        g_memory.data(),
        g_memory.size(),
        std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::uint8_t> vec{ &pool };

    for (std::int32_t i{}; i != 5; ++i) {
        vec.push_back(1 + i);
        dumpBuffer(std::span{ g_memory });
    }
}

static void test_pmr_10_04()
{
    // needing buffer size 32

    // Speicher mit reserve vorab reservieren

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource pool{
        g_memory.data(),
        g_memory.size(),
        std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::uint8_t> vec{ &pool };

    vec.reserve(5);

    for (std::int32_t i{}; i != 5; ++i) {
        vec.push_back(1 + i);
        dumpBuffer(std::span{ g_memory });
    }
}

static void test_pmr_10_05()
{
    // needing buffer size 32

    // bei elementaren Datentypen ist zwischen push_back und emplace_back kein Unterschied

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource pool{
        g_memory.data(),
        g_memory.size(),
        std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::uint8_t> vec{ &pool };

    vec.reserve(5);

    for (std::int32_t i{}; i != 5; ++i) {
        vec.emplace_back(1 + i);
        dumpBuffer(std::span{ g_memory });
    }
}

static void test_pmr_10_06()
{
    // needing buffer size 170

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource pool{
        g_memory.data(),
        g_memory.size(),
        std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::uint8_t> vec{ &pool };
    std::pmr::list<std::uint8_t> list{ &pool };
    std::pmr::set<std::uint8_t> set{ &pool };

    dumpBuffer(std::span{ g_memory });

    vec.push_back('A');
    dumpBuffer(std::span{ g_memory });

    list.push_back('B');
    dumpBuffer(std::span{ g_memory });

    set.insert('C');
    dumpBuffer(std::span{ g_memory });
}



// =====================================================================================

void test_pmr_10()
{
    //test_pmr_10_01();
    //test_pmr_10_02();
    //test_pmr_10_03();
    //test_pmr_10_04();
  //  test_pmr_10_05();
    test_pmr_10_06();
}

// =====================================================================================
// End-of-File
// =====================================================================================
