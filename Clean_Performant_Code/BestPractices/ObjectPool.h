// ===========================================================================
// ObjectPool.h // Best Practices
// ===========================================================================

// Vorraussetzungen: std::allocator<T>: NOCH NICHT BESCHRIEBEN

// placement new: IST BESCHRIEBEN

// std::launder IST BESCHRIEBEN
// https://stackoverflow.com/questions/39382501/what-is-the-purpose-of-stdlaunder

// Das muss ebenfalls rein:  IST BESCHRIEBEN
// https://en.cppreference.com/w/cpp/memory/destroy_at


// deleter NOCH NICHT BESCHRIEBEN

#pragma once

#include <cassert>
#include <memory>
#include <numeric>
#include <print>
#include <vector>

//// Provides an object pool that can be used with any class.
////
//// acquireObject() returns an object from the list of free objects. If
//// there are no more free objects, acquireObject() creates a new chunk
//// of objects.
//// The pool only grows: objects are never removed from the pool, until
//// the pool is destroyed.
//// acquireObject() returns an std::shared_ptr with a custom deleter that
//// automatically puts the object back into the object pool when the
//// shared_ptr is destroyed and its reference count reaches 0.
//
//template <typename T, typename TAllocator = std::allocator<T>>
//class ObjectPool final
//{
//public:
//    ObjectPool() = default;
//
//    explicit ObjectPool(const TAllocator& allocator) 
//        : m_allocator{ allocator } 
//    {}
//    
//    ~ObjectPool() {
//    
//        // Note: this implementation assumes that all objects handed out by this pool
//        // have been returned to the pool before the pool is destroyed.
//        // The following statement asserts if that is not the case.
//        assert(m_freeObjects.size() == ms_initialChunkSize * (std::pow(2, m_pool.size()) - 1));
//        
//        // Deallocate all allocated memory.
//        std::size_t chunkSize{ ms_initialChunkSize };
//        
//        for (auto* chunk : m_pool) {
//            m_allocator.deallocate(chunk, chunkSize);
//            chunkSize *= 2;
//        }
//
//        m_pool.clear();
//    }
//
//    // prevent move construction and move assignment.
//    ObjectPool(ObjectPool&&) = delete;
//    ObjectPool& operator=(ObjectPool&&) = delete;
//
//    // prevent copy construction and copy assignment.
//    ObjectPool(const ObjectPool&) = delete;
//    ObjectPool& operator=(const ObjectPool&) = delete;
//
//    // Reserves and returns an object from the pool. Arguments can be
//    // provided which are perfectly forwarded to a constructor of T.
//    template <typename... TArgs>
//    
//    std::shared_ptr<T> acquireObject(TArgs&& ... args) {
//
//        // If there are no free objects, allocate a new chunk.
//        if (m_freeObjects.empty()) { 
//            addChunk();
//        }
//
//        // Get a free object.
//        T* object{ m_freeObjects.back() };
//
//        // Initialize, i.e. construct, an instance of T in an
//        // uninitialized block of memory using placement new, and
//        // perfectly forward any provided arguments to the constructor.
//        ::new(object) T{ std::forward<TArgs>(args) ... };
//
//        // Launder the object pointer.
//        T* constructedObject{ std::launder(object) };
//
//        // Remove the object from the list of free objects.
//        m_freeObjects.pop_back();
//
//        auto deleter = [this](T* object) {
//            // Destroy object.
//            std::destroy_at(object);
//            // Put the object back in the list of free objects.
//            m_freeObjects.push_back(object);
//        };
//
//        return std::shared_ptr<T>{ constructedObject, deleter };
//
//        // Wrap the constructed object and return it.
//        //return std::shared_ptr<T>{constructedObject, [this](T* object) {
//        //    // Destroy object.
//        //    std::destroy_at(object);
//        //    // Put the object back in the list of free objects.
//        //    m_freeObjects.push_back(object);
//        //    }
//        //};
//    }
//
//private:
//    // Creates a new block of uninitialized memory,
//    //  big enough to hold m_newChunkSize instances of T.
//    void addChunk() {
//
//        std::println("Allocating new chunk...");
//
//        // Allocate a new chunk of uninitialized memory big enough to hold
//        // m_newChunkSize instances of T, and add the chunk to the pool.
//        // Care is taken that everything is cleaned up in the event of an exception.
//        // Hmmm .. das ist mir nicht ganz klar ??????
//        m_pool.push_back(nullptr);
//        try {
//            m_pool.back() = m_allocator.allocate(m_newChunkSize);  // back() returns a reference to the last element in the container.
//        }
//        catch (...) {
//            m_pool.pop_back();   // pop_back() removes the last element of the container
//            throw;
//        }
//
//        // Create pointers to each individual object in the new chunk
//        // and store them in the list of free objects.
//        auto oldFreeObjectsSize{ m_freeObjects.size() };
//        
//        m_freeObjects.resize(oldFreeObjectsSize + m_newChunkSize);
//        
//        auto tmp = m_pool.back();
//
//        std::iota(
//            begin(m_freeObjects) + oldFreeObjectsSize,
//            end(m_freeObjects), 
//            m_pool.back()
//        );
//
//        // double the chunk size for next time.
//        m_newChunkSize *= 2;
//    }
//
//    // Contains chunks of memory in which instances of T will be created.
//    // For each chunk, the pointer to its first object is stored.
//    std::vector<T*> m_pool;
//    
//    // Contains pointers to all free instances of T that
//    // are available in the pool.
//    
//    std::vector<T*> m_freeObjects;
//    // The number of T instances that should fit in the first allocated chunk.
//    
//    static constexpr std::size_t ms_initialChunkSize{ 5 };
//    // The number of T instances that should fit in a newly allocated chunk.
//    // This value is doubled after each newly created chunk.
//    
//    std::size_t m_newChunkSize{ ms_initialChunkSize };
//    
//    // The allocator to use for allocating and deallocating chunks.
//    TAllocator m_allocator;
//};

// ===========================================================================
// End-of-File
// ===========================================================================


// Provides an object pool that can be used with any class.
//
// acquireObject() returns an object from the list of free objects. If
// there are no more free objects, acquireObject() creates a new chunk
// of objects.
// The pool only grows: objects are never removed from the pool, until
// the pool is destroyed.
// acquireObject() returns an std::shared_ptr with a custom deleter that
// automatically puts the object back into the object pool when the
// shared_ptr is destroyed and its reference count reaches 0.
template <typename T, typename TAllocator = std::allocator<T>>
class ObjectPool final
{
private:
    // number of T instances that should fit in the first allocated chunk
    static constexpr std::size_t InitialChunkSize{ 1 };

public:
    // c'tor/d'tor
    ObjectPool();
    explicit ObjectPool(const TAllocator& allocator);
    ~ObjectPool();

    // prevent copy/move construction and copy/move assignment
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;
    ObjectPool& operator=(ObjectPool&&) = delete;

    // reserves and returns an object from the pool
    template <typename... TArgs>
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
ObjectPool<T, TAllocator>::ObjectPool() 
    : m_currentChunkSize{ InitialChunkSize }
{
}

template <typename T, typename TAllocator>
ObjectPool<T, TAllocator>::ObjectPool(const TAllocator& allocator)
    : m_currentChunkSize{ InitialChunkSize }, m_allocator{ allocator }
{
}

template <typename T, typename TAllocator>
ObjectPool<T, TAllocator>::~ObjectPool()
{
    // Note: this implementation assumes that all objects handed out by this
    //       pool have been returned to the pool before the pool is destroyed.
    //       The following statement asserts if that is not the case.
    assert(m_freeObjects.size() == InitialChunkSize * (std::pow(2, m_pool.size()) - 1));

    // Deallocate all allocated memory.
    std::size_t chunkSize{ InitialChunkSize };
    for (auto* chunk : m_pool) {
        m_allocator.deallocate(chunk, chunkSize);
        chunkSize *= 2;
    }
    m_pool.clear();
}

template <typename T, typename TAllocator>
template <typename... TArgs>
std::shared_ptr<T> ObjectPool<T, TAllocator>::acquireObject(TArgs&&... args)
{
    // if there are no free objects, need to allocate a new chunk
    if (m_freeObjects.empty()) { 
        addChunk();
    }

    // get a free object
    T* object{ m_freeObjects.back() };

    // Initialize, i.e. construct, an instance of T in an
    // uninitialized block of memory using placement new, and
    // perfectly forward any provided arguments to the constructor.
    ::new(object) T{ std::forward<TArgs>(args)... };

    // Launder the object pointer.
    T* constructedObject{ std::launder(object) };

    // Remove the object from the list of free objects.
    m_freeObjects.pop_back();

    // wrap the constructed object and return it
    return std::shared_ptr<T>{
        constructedObject,
        [this] (T* object) {
            std::destroy_at(object);          // destroy object
            m_freeObjects.push_back(object);  // put object back in the list of free objects.
        }
    };
}

template <typename T, typename TAllocator>
void ObjectPool<T, TAllocator>::addChunk()
{
    std::println("allocating new chunk ...");

    // Allocate a new chunk of uninitialized memory big enough to hold
    // m_newChunkSize instances of T, and add the chunk to the pool.
    // Care is taken that everything is cleaned up in the event of an exception.
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
    auto oldFreeObjectsSize{ m_freeObjects.size() };

    m_freeObjects.resize(oldFreeObjectsSize + m_currentChunkSize);

    std::iota(
        std::begin(m_freeObjects) + oldFreeObjectsSize, 
        std::end(m_freeObjects), 
        m_pool.back()
    );

    // double the chunk size for next time
    m_currentChunkSize *= 2;
}
