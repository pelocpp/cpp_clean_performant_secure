// ===========================================================================
// IsTranparent.cpp
// ===========================================================================

#include <cstdio>
#include <cstdlib>
#include <new>
#include <print>
#include <set>
#include <string>

// =================================================================================
// Demonstration of whether conversion takes place
// Variant: Count allocations.
// A temporary `std::string` performs an allocation if the text does not fit into the SSO buffer.
// Therefore, use a long string and overload the global `operator new`:

static std::size_t g_allocs = 0;

void* operator new(std::size_t n)
{
    ++g_allocs;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc{};
}

void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

template <class TSet>
static void run(const char* name)
{
    TSet s{};

    s.insert("A sufficiently long string that exceeds the SSO limit");

    const char* key = "A sufficiently long string that exceeds the SSO limit";

    auto before = g_allocs;
    auto pos = s.find(key);
    auto used = g_allocs - before;

    std::println("{:22} allocations in find: {} (found: {})", name, used, pos != s.end());
}

void test_is_transparent()
{
    run<std::set<std::string>>("std::less<std::string>");      // 1 allocations
    run<std::set<std::string, std::less<>>>("std::less<>");    // 0 allocations
}

// ===========================================================================
// End-of-File
// ===========================================================================
