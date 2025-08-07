// ===========================================================================
// FixedBlockMemoryManager.h //  // Performance Optimization Advanced
// ===========================================================================

//#include <iostream>
//#include <memory>
//#include <print>

template <class Arena>
struct fixed_block_memory_manager
{
    template <int N>
    fixed_block_memory_manager(char(&a)[N]);

    ~fixed_block_memory_manager() = default;

    // no copy / no move
    fixed_block_memory_manager(const fixed_block_memory_manager&) = delete;
    fixed_block_memory_manager& operator=(const fixed_block_memory_manager&) = delete;
    fixed_block_memory_manager(fixed_block_memory_manager&&) noexcept = delete;
    fixed_block_memory_manager& operator=(fixed_block_memory_manager&&) noexcept = delete;

    // public interface
    void* allocate(size_t);
    size_t block_size() const;
    size_t capacity() const;
    void   clear();
    void   deallocate(void*);
    bool   empty() const;

private:
    struct free_block {
        free_block* next;
    };

    free_block* free_ptr_;
    size_t      block_size_;
    Arena       arena_;
};

// ===========================================================================
// End-of-File
// ===========================================================================
