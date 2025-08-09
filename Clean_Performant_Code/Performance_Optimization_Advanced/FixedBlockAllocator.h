// ===========================================================================
// FixedBlockAllocator.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

// TBD:
// https://stackoverflow.com/questions/1553854/template-static-variable
// Wohin mit der Arena 

#include "FixedArenaController.h"
#include "FixedBlockMemoryManager.h"

#include <memory>
#include <print>

extern FixedBlockMemoryManager<FixedArenaController> memoryManager;

template<typename T>
class FixedBlockAllocator {
public:
    using value_type = T;  // needed for container classes

    FixedBlockAllocator() = default;

    // needed for container classes to store a copy of the allocator
    template<typename U>
    constexpr FixedBlockAllocator(const FixedBlockAllocator<U>&) noexcept {}

    // public interface
    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t) noexcept;

    template<typename U, typename... TArgs>
    void construct(U* ptr, TArgs&&... args);

    template<typename U>
    void destroy(U* p) noexcept;

    friend bool operator== (const FixedBlockAllocator&, const FixedBlockAllocator&);
    friend bool operator!= (const FixedBlockAllocator&, const FixedBlockAllocator&);
};

template <class T>
inline T* FixedBlockAllocator<T>::allocate(size_t n) {

    T* dummy{};

    size_t numBytes{ n * sizeof(T) };

    std::println("Allocating {} bytes", numBytes);

    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
        throw std::bad_array_new_length();
    }

    auto ptr{ static_cast<T*>(memoryManager.allocate(numBytes)) };

    return (ptr != nullptr) ? ptr : throw std::bad_alloc();
}

template <typename T>
inline void FixedBlockAllocator<T>::deallocate(T* p, std::size_t) noexcept
{
    std::println("Deallocating memory");

    memoryManager.deallocate(p);
}

template <typename T>
template<typename U, typename... TArgs>
inline void FixedBlockAllocator<T>::construct(U* ptr, TArgs&&... args)
{
    std::println("Constructing element");
    ::new(ptr) U(std::forward<TArgs>(args)...);
}

template <typename T>
template <typename U>
inline void FixedBlockAllocator<T>::destroy(U* p) noexcept
{
    std::println("Destroying element");
    p->~U();
}

template <typename T>
bool operator==(const FixedBlockAllocator<T>&, const FixedBlockAllocator<T>&) { return true; }

template <typename T>
bool operator!=(const FixedBlockAllocator<T>&, const FixedBlockAllocator<T>&) { return false; }

// ===========================================================================
// End-of-File
// ===========================================================================
