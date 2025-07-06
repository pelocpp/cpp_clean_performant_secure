// ===========================================================================
// CustomAllocator.cpp //  // Performance Optimization Advanced
// ===========================================================================

#include <iostream>
#include <memory>
#include <print>
#include <vector>

template<typename T>
class SimpleAllocator {
public:
    using value_type = T;

    SimpleAllocator() = default;

    template<typename U>
    constexpr SimpleAllocator(const SimpleAllocator<U>&) noexcept {}

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

    void deallocate(T* p, std::size_t) noexcept {
        ::operator delete(p);
    }

    template<typename U, typename... TArgs>
    void construct(U* p, TArgs&&... args) {
        new(p) U(std::forward<TArgs>(args)...);
    }

    template<typename U>
    void destroy(U* p) noexcept {
        p->~U();
    }

    friend bool operator==(const SimpleAllocator&, const SimpleAllocator&) { return true; }
    friend bool operator!=(const SimpleAllocator&, const SimpleAllocator&) { return false; }
};


// ===========================================================================
// End-of-File
// ===========================================================================
