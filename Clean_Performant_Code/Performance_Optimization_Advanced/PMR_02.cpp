// =====================================================================================
// PMR_02.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "PMR_DumpBuffer.h"

#include <array>
#include <cctype>
#include <cstddef>
#include <limits>
#include <memory_resource>
#include <print>
#include <span>

// =====================================================================================
// first example: std::pmr::monotonic_buffer_resource without fallback

static constexpr std::size_t NumBytes = 32;

static std::array<std::uint8_t, NumBytes> g_memory{};

static void test_pmr_02_01()
{
    std::println("Address of current buffer:    {:#X}", reinterpret_cast<intptr_t>(g_memory.data()));

    std::fill(g_memory.begin(), g_memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::monotonic_buffer_resource res{ g_memory.data(), g_memory.size(), nullptr };

    std::uint32_t* ptr{ static_cast<std::uint32_t*> (res.allocate(sizeof(std::uint32_t), alignof (std::uint32_t))) };

    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 123;
    res.deallocate(ptr, alignof (std::uint32_t));

    ptr = static_cast<std::uint32_t*> (res.allocate(sizeof(std::uint32_t), alignof (std::uint32_t)));
    std::println("Pointer to allocated storage: {:#X}", reinterpret_cast<intptr_t>(ptr));
    *ptr = 456;
    res.deallocate(ptr, alignof (std::uint32_t));

    std::span span{ g_memory };
    dumpBuffer(span);
}

// =====================================================================================
// second example: looking at upstream resources

static void test_pmr_02_02()
{
    std::pmr::monotonic_buffer_resource res{ g_memory.data(), g_memory.size() };

    std::pmr::memory_resource* upstream{ res.upstream_resource() };

    std::pmr::memory_resource* resource{ std::pmr::new_delete_resource() };

    if (upstream == resource) {
        std::println("Default resource equals std::pmr::new_delete_resource");
    }
}

void test_pmr_02()
{
    test_pmr_02_01();
    test_pmr_02_02();
}

// =====================================================================================
// End-of-File
// =====================================================================================
