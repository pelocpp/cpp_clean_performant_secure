// ===========================================================================
// ObjectPool_FixedSize.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

#include <atomic>
#include <new>

namespace FixedSizeObjectPool {

    template<class T, size_t Size = 3>
    class ObjectPool final
    {
    public:
        using value_type = T;

        // c'tor/d'tor
        ObjectPool();
        ~ObjectPool();

        // no copy / no move
        ObjectPool(const ObjectPool&) = delete;
        ObjectPool& operator =(const ObjectPool&) = delete;
        ObjectPool(ObjectPool&& other) noexcept = delete;
        ObjectPool& operator= (ObjectPool&& other) noexcept = delete;

        [[nodiscard]] T* allocate();
        void deallocate(T* p) noexcept;

        template<typename ...TArgs>
        [[nodiscard]] T* construct(TArgs&& ...args);
        void destroy(T* p) noexcept;

        size_t size() noexcept;
        size_t capacity() noexcept;

    private:
        struct FreeList
        {
            FreeList* m_next;
        };

        FreeList*  m_pool;
        FreeList*  m_nextFree;
        size_t     m_size;
        size_t     m_capacity;
    };

    template <typename T, size_t Size>
    inline ObjectPool<T, Size>::ObjectPool()
        : m_pool{ nullptr }, m_nextFree{ nullptr }, m_size{ Size }, m_capacity{ Size }
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

        --m_capacity;
        return reinterpret_cast<T*>(item);
    }

    template <typename T, size_t Size>
    inline void ObjectPool<T, Size>::deallocate(T* ptr) noexcept
    {
        const auto item = reinterpret_cast<FreeList*>(ptr);

        item->m_next = m_nextFree;
        m_nextFree = item;

        ++m_capacity;
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

    template <typename T, size_t Size>
    size_t ObjectPool<T, Size>::size() noexcept
    {
        return m_size;
    }

    template <typename T, size_t Size>
    size_t ObjectPool<T, Size>::capacity() noexcept
    {
        return m_capacity;
    }
}

// ===========================================================================
// End-of-File
// ===========================================================================
