// ===========================================================================
// CustomAllocator.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

#include <memory>
#include <print>

template<typename T>
class CustomAllocator {
public:
    using value_type = T;

    CustomAllocator() = default;

    // The reason that allocators must be copy constructible is
    // that container classes store a copy of the allocator
    template<typename U>
    constexpr CustomAllocator(const CustomAllocator<U>&) noexcept {}

    T* allocate(std::size_t n)
    {
        size_t numBytes{ n * sizeof(T) };

        std::println("Allocating {} bytes", numBytes);

        if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
            throw std::bad_array_new_length();

        if (auto p = static_cast<T*>(std::malloc(numBytes)))
        {
            return p;
        }

        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept
    {
        std::println("Deallocating memory");
        std::free(p);
    }

    template<typename U, typename... TArgs>
    void construct(U* p, TArgs&&... args)
    {
        std::println("Constructing element");
        new(p) U(std::forward<TArgs>(args)...);
    }

    template<typename U>
    void destroy(U* p) noexcept
    {
        std::println("Destroying element");
        p->~U();
    }

    friend bool operator==(const CustomAllocator&, const CustomAllocator&) { return true; }
    friend bool operator!=(const CustomAllocator&, const CustomAllocator&) { return false; }
};

// ===========================================================================
// End-of-File
// ===========================================================================
