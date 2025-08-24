// ===========================================================================
// ObjectPool_DynamicSize.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

#include <cassert>
#include <memory>
#include <numeric>
#include <print>
#include <vector>

namespace DynamicSizeObjectPool {

    template <typename T, typename TAllocator = std::allocator<T>>
    class ObjectPool final
    {
    public:
        using value_type = T;

    private:
        // number of T instances that should fit in the first allocated chunk
        static constexpr std::size_t InitialChunkSize{ 2 };

    public:
        // c'tor / d'tor
        ObjectPool();
        explicit ObjectPool(const TAllocator& allocator);

        ~ObjectPool();

        // no copy / no move
        ObjectPool(const ObjectPool&) = delete;
        ObjectPool(ObjectPool&&) = delete;
        ObjectPool& operator=(const ObjectPool&) = delete;
        ObjectPool& operator=(ObjectPool&&) = delete;

        // reserves and returns an object from the pool
        template <typename ... TArgs>
        std::shared_ptr<T> acquireObject(TArgs&&... args);

    private:
        // private helper methods
        void addChunk();

        // member data
        std::vector<T*>  m_pool;
        std::vector<T*>  m_freeObjects;
        std::size_t      m_currentChunkSize;
        TAllocator       m_allocator;
    };

    template <typename T, typename TAllocator>
    inline ObjectPool<T, TAllocator>::ObjectPool()
        : m_currentChunkSize{ InitialChunkSize }
    {
    }

    template <typename T, typename TAllocator>
    inline ObjectPool<T, TAllocator>::ObjectPool(const TAllocator& allocator)
        : m_currentChunkSize{ InitialChunkSize }, m_allocator{ allocator }
    {
    }

    template <typename T, typename TAllocator>
    inline ObjectPool<T, TAllocator>::~ObjectPool()
    {
        // Note: this implementation assumes that all objects handed out by this
        //       pool have been returned to the pool before the pool is destroyed.
        //       The following statement asserts if that is not the case.
        assert(m_freeObjects.size() == InitialChunkSize * (std::pow(2, m_pool.size()) - 1));

        // deallocate all allocated memory
        std::size_t chunkSize{ InitialChunkSize };
        for (auto* chunk : m_pool) {
            m_allocator.deallocate(chunk, chunkSize);
            chunkSize *= 2;
        }
        m_pool.clear();
    }

    template <typename T, typename TAllocator>
    template <typename... TArgs>
    inline std::shared_ptr<T> ObjectPool<T, TAllocator>::acquireObject(TArgs&& ... args)
    {
        // if there are no free objects, need to allocate a new chunk
        if (m_freeObjects.empty()) {
            addChunk();
        }

        // get a free object
        T* object{ m_freeObjects.back() };

        // initialize, i.e. construct, an instance of T in an
        // uninitialized block of memory using placement new, and
        // perfectly forward any provided arguments to the constructor.
        ::new(object) T{ std::forward<TArgs>(args)... };
        // or
        // std::construct_at(object, std::forward<TArgs>(args) ...);

        // launder the object pointer
        T* constructedObject{ std::launder(object) };

        // remove the object from the list of free objects
        m_freeObjects.pop_back();

        // wrap the constructed object and return it
        return std::shared_ptr<T>{
            constructedObject,
            [this](T* object) {
                std::destroy_at(object);          // destroy object
                m_freeObjects.push_back(object);  // put object back in the list of free objects.
            }
        };
    }

    template <typename T, typename TAllocator>
    inline void ObjectPool<T, TAllocator>::addChunk()
    {
        std::println("allocating new chunk ...");

        // allocate a new chunk of uninitialized memory big enough to hold
        // m_newChunkSize instances of T, and add the chunk to the pool.
        // (care is taken that everything is cleaned up in the event of an exception)
        m_pool.push_back(nullptr);
        try {
            m_pool.back() = m_allocator.allocate(m_currentChunkSize);
        }
        catch (...) {
            m_pool.pop_back();
            throw;
        }

        // Create pointers to each individual object in the new chunk
        // and store them in the list of free objects.
        // This list is normally always empty;
        // the implementation also works if the list still contains elements
        auto oldFreeObjectsSize{ m_freeObjects.size() };

        m_freeObjects.resize(oldFreeObjectsSize + m_currentChunkSize);

        // create pointers to the new instances of T (using std::iota)
        std::iota(
            std::begin(m_freeObjects) + oldFreeObjectsSize,
            std::end(m_freeObjects),
            m_pool.back()
        );

        // alternative implementation to setup m_freeObjects list
        //for (std::size_t i{}; i != m_currentChunkSize; ++i) {
        //    m_freeObjects[oldFreeObjectsSize + i] = m_pool.back() + i;
        //}

        // double the chunk size for next time
        m_currentChunkSize *= 2;
    }
}


// ===========================================================================
// End-of-File
// ===========================================================================
