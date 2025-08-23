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
            // std::aligned_storage_t<sizeof(value_type), alignof(value_type)> mStorage;
           // alignas (T)   std::byte m_storage;
            //alignas (T) T m_storage;
            FreeList* m_next;
        };

    private:
        //std::unique_ptr<Item[]> m_pool;
        FreeList* m_pool;
        FreeList* m_nextFree;
        size_t     m_size;

        //std::unique_ptr<Item[]> mPool = std::make_unique<Item[]>(Size);
        //Item* m_nextFree = nullptr;
    };

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
