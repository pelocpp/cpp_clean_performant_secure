// ===========================================================================
// ObjectPool_FixedSize.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

//#include <cassert>
//#include <memory>
//#include <numeric>
//#include <print>
//#include <vector>

// Zentrale Eigenschaften dieser Realisierung:
// == Der Pool kann für jede Klasse verwendet werden
// == Der Pool wächst, wenn eine Anforderung nicht erfüllt werden kann.
// 

namespace FixedSizeObjectPool {

    template<class T, size_t Size = 3>
    class ObjectPool final
    {
    public:
        //  const static std::size_t Size = 100000;
     //    const static std::size_t Size = 3;

        using value_type = T;

        // c'tor/d'tor
        ObjectPool();
        ~ObjectPool();

        // nor copy nor assignment
        ObjectPool(const ObjectPool&) = delete;
        ObjectPool& operator =(const ObjectPool&) = delete;

        // support moving
        ObjectPool(ObjectPool&& other) noexcept;
        ObjectPool& operator= (ObjectPool&& other) noexcept;

        [[nodiscard]] T* allocate();
        void deallocate(T* p) noexcept;

        template<typename ...TArgs>
        [[nodiscard]] T* construct(TArgs&& ...args);
        void destroy(T* p) noexcept;

    public:
        struct FreeList
        {
            FreeList* m_next;
        };

    private:
        FreeList*  m_pool;
        FreeList*  m_nextFree;
        size_t     m_size;
    };

    template <typename T, size_t Size>
    inline ObjectPool<T, Size>::ObjectPool()
        : m_pool{}, m_nextFree{}, m_size{ Size }
    {
        size_t blockSize = std::max(sizeof(T), sizeof(void*));

        auto* storage = std::malloc(m_size * blockSize);
        if (storage == nullptr) {
            throw std::bad_alloc{};
        }

        m_pool = reinterpret_cast<FreeList*>(storage);

        // setup list of free blocks pointers (within allocated memory arena)
        std::byte* ptr = reinterpret_cast<std::byte*>(m_pool);
        for (size_t count = m_size; count > 1; --count, ptr += blockSize) {
            *reinterpret_cast<std::byte**>(ptr) = ptr + blockSize;

            //std::println("{:#X} => {:#X}",
            //    reinterpret_cast<intptr_t>(ptr),
            //    reinterpret_cast<intptr_t>(ptr + blockSize)
            //);
        }

        *reinterpret_cast<std::byte**>(ptr) = nullptr;

        m_nextFree = &m_pool[0];

    }

    template <typename T, size_t Size>
    inline ObjectPool<T, Size>::~ObjectPool()
    {
        void* storage = reinterpret_cast<void*>(m_pool);
        delete[] storage;
    }

    template <typename T, size_t Size>
    [[nodiscard]] inline T* ObjectPool<T, Size>::allocate()
    {
        if (m_nextFree == nullptr) {
            throw std::bad_alloc{};
        }

        const auto item = m_nextFree;
        m_nextFree = item->m_next;

        return reinterpret_cast<T*>(item);
    }

    template <typename T, size_t Size>
    inline void ObjectPool<T, Size>::deallocate(T* ptr) noexcept
    {
        const auto item = reinterpret_cast<FreeList*>(ptr);

        item->m_next = m_nextFree;
        m_nextFree = item;
    }

    template <typename T, size_t Size>
    template<typename ...TArgs>
    [[nodiscard]] inline T* ObjectPool<T, Size>::construct(TArgs&& ...args)
    {
        T* ptr = allocate();
        std::construct_at(ptr, std::forward<TArgs>(args)...);
        return ptr;
    }

    template <typename T, size_t Size>
    inline void ObjectPool<T, Size>::destroy(T* ptr) noexcept
    {
        if (ptr == nullptr) {
            return;
        }

        std::destroy_at(ptr);
        deallocate(ptr);
    }

    // move semantics
    template <typename T, size_t Size>
    inline ObjectPool<T, Size>& ObjectPool<T, Size>::operator= (ObjectPool&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }

        m_pool = other.m_pool;
        m_nextFree = other.m_nextFree;
        m_size = other.m_size;

        other.m_pool = nullptr;
        other.m_nextFree = nullptr;
        other.m_size = 0;

        return *this;
    }

    template <typename T, size_t Size>
    inline ObjectPool<T, Size>::ObjectPool(ObjectPool&& other) noexcept
        : m_pool{ other.m_pool }
        , m_nextFree{ other.m_nextFree }
        , m_size{ other.m_size }
    {
        other.m_pool = nullptr;
        other.m_nextFree = nullptr;
        other.m_size = 0;
    }
}

// ===========================================================================
// End-of-File
// ===========================================================================
