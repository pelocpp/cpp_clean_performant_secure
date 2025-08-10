// ===========================================================================
// FixedBlockMemoryManager.h // Performance Optimization Advanced
// ===========================================================================

#pragma once

// TBD:
// Blockgröße über einen Template Parameter einbringen ... 

#include "FixedArenaController.h"

template <typename TArena>
class FixedBlockMemoryManager
{
public:
    template <size_t N>
    FixedBlockMemoryManager(char(&a)[N]);

    ~FixedBlockMemoryManager() = default;

    // no copy / no move
    FixedBlockMemoryManager(const FixedBlockMemoryManager&) = delete;
    FixedBlockMemoryManager& operator=(const FixedBlockMemoryManager&) = delete;
    FixedBlockMemoryManager(FixedBlockMemoryManager&&) noexcept = delete;
    FixedBlockMemoryManager& operator=(FixedBlockMemoryManager&&) noexcept = delete;

    // public interface
    void*  allocate(size_t);
    size_t blockSize() const;
    size_t capacity() const;
    void   clear();
    void   deallocate(void*);
    bool   empty() const;

private:
    struct free_block {
        free_block* next;
    };

    free_block* m_freePtr;
    size_t      m_blockSize;
    TArena      m_arena;
};

template <typename TArena>
template <size_t N>
inline FixedBlockMemoryManager<TArena>::FixedBlockMemoryManager(char(&a)[N]) :
    m_arena{ a }, m_freePtr{ nullptr }, m_blockSize{ 0 }
{
    std::println("FixedBlockMemoryManager: N = {}", N);
}

template <typename TArena>
inline void* FixedBlockMemoryManager<TArena>::allocate(size_t size) {

    if (empty()) {
        m_freePtr = reinterpret_cast<free_block*>(m_arena.allocate(size));

        m_blockSize = m_arena.blockSize();

        if (empty())
            throw std::bad_alloc();
    }
    if (size > m_blockSize)        // Hmmm, want to respect nodes of different size
        throw std::bad_alloc();

    auto p = m_freePtr;
    m_freePtr = m_freePtr->next;
    return p;
}

template <typename TArena>
inline void FixedBlockMemoryManager<TArena>::deallocate(void* p) {
    if (p == nullptr)
        return;
    auto fp = reinterpret_cast<free_block*>(p);
    fp->next = m_freePtr;
    m_freePtr = fp;
}

template <typename TArena>
inline size_t FixedBlockMemoryManager<TArena>::capacity() const {
    return m_arena.capacity();
}

template <typename TArena>
inline void FixedBlockMemoryManager<TArena>::clear() {
    m_freePtr = nullptr;
    m_arena.clear();
}

template <typename TArena>
inline bool FixedBlockMemoryManager<TArena>::empty() const {
    return m_freePtr == nullptr;
}

template <typename TArena>
inline size_t FixedBlockMemoryManager<TArena>::blockSize() const {

    return m_blockSize;
}

// ===========================================================================
// End-of-File
// ===========================================================================
