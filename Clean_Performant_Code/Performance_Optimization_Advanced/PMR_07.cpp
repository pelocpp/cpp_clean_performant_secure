// =====================================================================================
// PMR_07.cpp // Polymorphic Memory Resources
// =====================================================================================

#include "PMR_DumpBuffer.h"

#include <array>
#include <cctype>
#include <cstddef>
#include <initializer_list>
#include <limits>
#include <memory_resource>
#include <print>
#include <span>
#include <string_view>
#include <vector>

// =====================================================================================
// Testing 
// =====================================================================================

class NewDelete_ResourceTracker : public std::pmr::memory_resource
{
public:
    NewDelete_ResourceTracker() {
        std::println("New Delete Resource Tracker:");
    }

    ~NewDelete_ResourceTracker() {
        std::println("New Delete Resource Tracker: Done.");
    }

private:
    void* do_allocate(std::size_t bytes, std::size_t alignment) override {
        std::println("## allocating {} bytes", bytes);
        return std::pmr::new_delete_resource()->allocate(bytes, alignment);
    }

    void do_deallocate(void* p, std::size_t bytes, std::size_t alignment) override {
        std::println("## deallocating {} bytes", bytes);
        std::pmr::new_delete_resource()->deallocate(p, bytes, alignment);
    }

    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override {
        std::println("## do_is_equal");
        return this == &other;
    }
};

// =====================================================================================
// Works fine - emplace_back

static void test_pmr_07_01()
{
    NewDelete_ResourceTracker tracker;
    std::pmr::set_default_resource(&tracker);

    constexpr std::size_t NumBytes = 160;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::fill(memory.begin(), memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::vector<std::pmr::string> strings{ &resource };

    strings.reserve(2);

    strings.emplace_back("A lazy dog");
    strings.emplace_back("Another dog");

    dumpBuffer(std::span{ memory });
}

// =====================================================================================
// Works fine - emplace_back

static void test_pmr_07_02()
{
    NewDelete_ResourceTracker tracker;
    std::pmr::set_default_resource(&tracker);

    constexpr std::size_t NumBytes = 256;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::fill(memory.begin(), memory.end(), std::numeric_limits<std::uint8_t>::max());

    std::pmr::vector<std::pmr::string> strings{ &resource };

    strings.reserve(2);

    strings.emplace_back("The quick brown fox jumps over the lazy dog");
    strings.emplace_back("Lorem ipsum dolor sit amet, consectetur adipiscing elit");

    dumpBuffer(std::span{ memory });
}

// =====================================================================================
// Issues with push_back

static void test_pmr_07_03()
{
    NewDelete_ResourceTracker tracker;
    std::pmr::set_default_resource(&tracker);

    constexpr std::size_t NumBytes = 1024;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::pmr::string> strings{ &resource };

    // --------------------------------------------------------------------------------
    // Works fine
    //strings.emplace_back("A lazy dog");
    //strings.emplace_back("Another dog");

    // --------------------------------------------------------------------------------
    // Works fine too
    //strings.emplace_back("The quick brown fox jumps over the lazy dog");
    //strings.emplace_back("Lorem ipsum dolor sit amet, consectetur adipiscing elit");

    // --------------------------------------------------------------------------------
    // No: std::pmr::new_delete_resource is used !!!
    //strings.push_back("A lazy dog");
    //strings.push_back("Another dog");

    // --------------------------------------------------------------------------------
    // Again No: std::pmr::new_delete_resource is used !!!
    // strings.push_back("The quick brown fox jumps over the lazy dog");
    // strings.push_back("Lorem ipsum dolor sit amet, consectetur adipiscing elit");

    // --------------------------------------------------------------------------------
    // Could work, but does not:
    // std::pmr::string objects do not have any knowledge of the surrounding containers memory resource
    //std::pmr::string s1{ "A lazy dog" };
    //std::pmr::string s2{ "Another dog" };

    //strings.push_back(s1);
    //strings.push_back(s2);


    // --------------------------------------------------------------------------------
    // Works fine

    std::pmr::string s1{ "A lazy dog", &resource };
    std::pmr::string s2{ "Another dog", &resource };

    std::pmr::string s3{ "The quick brown fox jumps over the lazy dog", &resource };
    std::pmr::string s4{ "Lorem ipsum dolor sit amet, consectetur adipiscing elit", &resource };

    strings.push_back(s1);
    strings.push_back(s2);
    strings.push_back(s3);
    strings.push_back(s4);
}

// =====================================================================================
// Issues with std::initializer_list

static void test_pmr_07_04()
{
    NewDelete_ResourceTracker tracker;
    std::pmr::set_default_resource(&tracker);

    constexpr std::size_t NumBytes = 512;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    // --------------------------------------------------------------------------------
    // No: std::pmr::new_delete_resource is used !!!
    // std::pmr::vector<std::pmr::string> strings{ "Hello", "World" };

    // --------------------------------------------------------------------------------
    // No: std::pmr::new_delete_resource is used !!!
    // std::pmr::vector<std::pmr::string> strings{ { std::pmr::string {"Hello"},  std::pmr::string{"World"} }, & resource };

    // --------------------------------------------------------------------------------
    // No: std::pmr::new_delete_resource is used !!!
    // std::pmr::vector<std::pmr::string> strings{ {"Hello", "World" }, &resource };

    // --------------------------------------------------------------------------------
    // No: std::pmr::new_delete_resource is used !!!
    // And don't forget: It's not intended by design,
    // to create an std::initializer_list object explicitely (!)
    std::initializer_list<std::pmr::string> list{
        std::pmr::string {"Hello", &resource},
        std::pmr::string{"World", &resource}
    };

    std::pmr::vector<std::pmr::string> strings{ list.begin(), list.end() };

}

// =====================================================================================
// Issues with std::initializer_list solved - more or less with a separate utility function

template <typename TContainer, typename ... TValues>
auto createContainer(auto* resource, TValues&&... values)
{
    TContainer result{ resource };
    result.reserve(sizeof... (values));
    (result.emplace_back(std::forward<TValues>(values)), ...);
    return result;
};

static void test_pmr_07_05()
{
    NewDelete_ResourceTracker tracker;
    std::pmr::set_default_resource(&tracker);

    constexpr std::size_t NumBytes = 1024;

    std::array<std::uint8_t, NumBytes> memory{};

    std::pmr::monotonic_buffer_resource resource{
        memory.data(), memory.size(), std::pmr::null_memory_resource()
    };

    std::pmr::vector<std::pmr::string> vec{
        createContainer<std::pmr::vector<std::pmr::string>>(
            &resource,
            "Hello",
            "World",
            "The quick brown fox jumps over the lazy dog",
            "Lorem ipsum dolor sit amet, consectetur adipiscing elit"
        )
    };

    for (std::size_t i{}; const auto& s : vec) {
        std::println("{}: {}", i, s);
        ++i;
    }
}

// =====================================================================================

void test_pmr_07()
{
    test_pmr_07_01();
    test_pmr_07_02();
    test_pmr_07_03();
    test_pmr_07_04();
    test_pmr_07_05();
}

// =====================================================================================
// End-of-File
// =====================================================================================
