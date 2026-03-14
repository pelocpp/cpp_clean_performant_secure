// =====================================================================================
// PMR_04.cpp // Polymorphic Memory Resources
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

static constexpr std::size_t NumBytes = 176;

static std::array<std::uint8_t, NumBytes> g_memory{};
 
// =====================================================================================
// First example: std::pmr::vector<std::uint8_t> with push_back

static void test_pmr_04_01()
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

// =====================================================================================
// Second example: with method 'reserve'

static void test_pmr_04_02()
{
    // needing buffer size 32

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

// =====================================================================================
// Third example: std::pmr::vector<std::uint8_t> with emplace_back
// No difference compared with push_back

static void test_pmr_04_03()
{
    // needing buffer size 32

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


// =====================================================================================
// Fourth example: several STL container objects using the same memory resource object

static void test_pmr_04_04()
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

void test_pmr_04()
{
    test_pmr_04_01();
    test_pmr_04_02();
    test_pmr_04_03();
    test_pmr_04_04();
}

// =====================================================================================
// End-of-File
// =====================================================================================
