// =====================================================================================
// PMR_06.cpp
// =====================================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <cstddef>
#include <memory_resource>
#include <print>

#include <array>
#include <cstddef>
#include <cstdint>
#include <memory_resource>
#include <random>
#include <string>
#include <vector>

// =====================================================================================
// Implementing a custom memory resource
// (from "High Performance C++", Viktor Sehr, Bjoern Andrist)

class LoggingResource : public std::pmr::memory_resource
{
private:
    std::pmr::memory_resource* m_res;

public:
    LoggingResource()
        : m_res{ std::pmr::null_memory_resource() }
    {
    }

    LoggingResource(std::pmr::memory_resource* res)
        : m_res{ res }
    {
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        auto* ptr{ m_res->allocate(bytes, alignment) };
        std::println("[do_allocate]   {:6} bytes (alignment {}) at   {:#X}", bytes, alignment, reinterpret_cast<intptr_t>(ptr));
        return ptr;
    }

    void do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override {
        std::println("[do_deallocate] {:6} bytes (alignment {}) from {:#X}", bytes, alignment, reinterpret_cast<intptr_t>(ptr));
        m_res->deallocate(ptr, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        std::println("[do_is_equal]");
        return this == &other;
    }
};

static void test_pmr_06_01_logging()
{
    constexpr std::size_t NumBytes = 128;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    LoggingResource loggingResource{ &resource };

    std::pmr::vector<std::uint32_t> numbers{ &loggingResource };

    numbers.reserve(5);

    for (std::uint32_t n{}; n != 5; ++n)
    {
        std::uint32_t value{ 2 * (n + 1) };
        std::println("before push_back {}", value);
        numbers.push_back(value);
    }

    numbers.clear();
}

// =====================================================================================

class FixedArenaResource : public std::pmr::memory_resource
{
private:
    std::uint8_t* m_begin;
    std::uint8_t* m_current;
    std::uint8_t* m_end;

public:
    FixedArenaResource(void* buffer, std::size_t size) noexcept :
        m_begin{ static_cast<std::uint8_t*>(buffer) },
        m_current{ m_begin },
        m_end{ m_begin + size }
    {
    }

    void reset() noexcept
    {
        m_current = m_begin;
    }

    std::size_t used() const noexcept
    {
        return static_cast<std::size_t>(m_current - m_begin);
    }

    std::size_t capacity() const noexcept
    {
        return static_cast<std::size_t>(m_end - m_begin);
    }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        std::uint8_t* aligned{ alignAddress(m_current, alignment) };

        if (aligned + bytes > m_end) {
            throw std::bad_alloc();  // no upstream resource
        }

        m_current = aligned + bytes;
        return aligned;
    }

    void do_deallocate(void*, std::size_t, std::size_t) override
    {
        // Arena based behaviour — no deallocation
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override
    {
        return this == &other;
    }

private:
    static std::uint8_t* alignAddress(std::uint8_t* ptr, std::size_t alignment)
    {
        auto addr{ reinterpret_cast<std::uintptr_t>(ptr) };
        auto aligned{ (addr + alignment - 1) & ~(alignment - 1) };
        return reinterpret_cast<std::uint8_t*>(aligned);
    }

    static std::uint8_t* alignAddressModulo(std::uint8_t* ptr, std::size_t alignment)
    {
        // If you find the bitmask logic difficult to read, you can use modulo.
        // It's slightly slower, but often easier to understand.

        auto addr{ reinterpret_cast<std::uintptr_t>(ptr) };
        auto remainder{ addr % alignment };
        if (remainder != 0) {
            addr += (alignment - remainder);
        }
        return reinterpret_cast<std::uint8_t*>(addr);
    }
};

// =====================================================================================

#ifdef _DEBUG
static constexpr std::size_t VectorCapacity{ 2000 };  // debug 
static constexpr std::size_t Iterations{ 200 };       // debug
#else
static constexpr std::size_t VectorCapacity{ 2000 };  // release 
static constexpr std::size_t Iterations{ 500 };       // release
#endif

// =====================================================================================

static std::mt19937 g_rng{ 123 };

static std::uniform_int_distribution<int> g_dist{ 24, 64 };

static inline std::size_t random_length()
{
    return g_dist(g_rng);
}

// =====================================================================================

static void test_pmr_06_02_benchmark_std()
{
    for (std::size_t n{}; n != Iterations; ++n)
    {
        std::vector<std::string> vec;

        vec.reserve(VectorCapacity);

        for (std::size_t i{}; i != VectorCapacity; ++i)
        {
            std::size_t len{ random_length() };

            std::string s{};
            s.reserve(len);

            for (std::size_t j{}; j != len; ++j) {
                s.push_back('a');
            }

            vec.push_back(std::move(s));
        }

        vec.clear();
    }
}

// =====================================================================================

static std::array<std::uint8_t, 18 * 1024 * 1024> backing{};

static void test_pmr_06_02_benchmark_pmr()
{
    FixedArenaResource pool{ backing.data(), backing.size() };

    for (std::size_t n{}; n != Iterations; ++n)
    {
        pool.reset();

        std::pmr::vector<std::pmr::string> vec{ &pool };

        vec.reserve(VectorCapacity);

        for (std::size_t i{}; i != VectorCapacity; ++i)
        {
            std::size_t len{ random_length() };

            std::pmr::string& s = vec.emplace_back();

            s.reserve(len);

            for (std::size_t j{}; j != len; ++j) {
                s.push_back('a');
            }
        }

        vec.clear();
    }
}

static void test_pmr_06_02_benchmark()
{
    std::cout << "Start" << std::endl;

    {
        ScopedTimer watch{};
        test_pmr_06_02_benchmark_std();
    }

    {
        ScopedTimer watch{};
        test_pmr_06_02_benchmark_pmr();
    }
}

void test_pmr_06()
{
    test_pmr_06_01_logging();
    test_pmr_06_02_benchmark();
}

// =====================================================================================
// End-of-File
// =====================================================================================
