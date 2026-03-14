// =====================================================================================
// PMR_08.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "../LoggerUtility/ScopedTimer.h"

#include <array>
#include <cstddef>
#include <memory_resource>
#include <print>
#include <random>
#include <string>
#include <vector>

// =====================================================================================

class TrackingResource : public std::pmr::memory_resource
{
private:
    std::pmr::memory_resource* m_upstream;

    std::size_t m_countAllocations;
    std::size_t m_countDeallocations;
    std::size_t m_bytesAllocated;

public:
    TrackingResource(std::pmr::memory_resource* upstream)
        : m_upstream{ upstream }, m_countAllocations{ 0 },
        m_countDeallocations{ 0 }, m_bytesAllocated{ 0 }
    {
    }

public:
    std::size_t getAllocations() const {
        return m_countAllocations;
    }

    std::size_t getDeallocations() const {
        return m_countDeallocations;
    }

    std::size_t getBytesAllocated() const {
        return m_bytesAllocated;
    }

protected:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        ++m_countAllocations;
        m_bytesAllocated += bytes;

        return m_upstream->allocate(bytes, alignment);
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override
    {
        ++m_countDeallocations;

        m_upstream->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }
};

// =====================================================================================

#ifdef _DEBUG
static constexpr std::size_t VectorCapacity{ 3000 }; // debug 
static constexpr std::size_t Iterations{ 200 };      // release

// just for testing
//static constexpr std::size_t VectorCapacity{ 50 }; // debug 
//static constexpr std::size_t Iterations{ 1 };      // release

#else
static constexpr std::size_t VectorCapacity{ 3000 }; // debug 
static constexpr std::size_t Iterations{ 1000 };     // release

#endif

// =====================================================================================

static std::mt19937 g_rng{ 123 };

static std::uniform_int_distribution<int> g_dist{ 24, 64 };

static inline std::size_t random_length()
{
    return g_dist(g_rng);
}

// =====================================================================================

static void benchmark_std()
{
    for (std::size_t n{}; n != Iterations; ++n)
    {
        std::vector<std::string> vec;
        vec.reserve(VectorCapacity);

        for (std::size_t i{}; i != VectorCapacity; ++i)
        {
            std::size_t len{ random_length() };

            std::string& s{ vec.emplace_back() };

            s.reserve(len);

            for (std::size_t j{}; j != len; ++j) {
                s.push_back('a');
            }
        }

        vec.clear();
    }
}

// =====================================================================================

static std::array<std::uint8_t, 18 * 1024 * 1024> g_memory{};

static void benchmark_pmr()
{
    // track heap usage
    TrackingResource tracker{ std::pmr::new_delete_resource() };

    for (std::size_t n{}; n != Iterations; ++n)
    {
        std::pmr::monotonic_buffer_resource pool{ g_memory.data(), g_memory.size(), &tracker };

        std::pmr::vector<std::pmr::string> vec{ &pool };

        vec.reserve(VectorCapacity);

        for (std::size_t i{}; i != VectorCapacity; ++i)
        {
            std::size_t len{ random_length() };

            std::pmr::string& s{ vec.emplace_back() };

            s.reserve(len);

            for (std::size_t j{}; j != len; ++j) {
                s.push_back('a');
            }
        }

        vec.clear();
    }

    std::println("Heap Allocations:     {}", tracker.getAllocations());
    std::println("Heap Deallocations:   {}", tracker.getDeallocations());
    std::println("Heap Bytes allocated: {}", tracker.getBytesAllocated());
}

static void test_pmr_08_01_benchmark()
{
    std::cout << "Start" << std::endl;

    {
        ScopedTimer watch{};
        benchmark_std();
    }

    {
        ScopedTimer watch{};
        benchmark_pmr();
    }
}

void test_pmr_08()
{
    test_pmr_08_01_benchmark();
}

// =====================================================================================
// End-of-File
// =====================================================================================
