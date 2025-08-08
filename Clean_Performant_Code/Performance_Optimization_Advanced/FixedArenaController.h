// ===========================================================================
// FixedArenaController.h //  // Performance Optimization Advanced
// ===========================================================================

#include <algorithm>
#include <print>

// TBD: rename   allocate ==> initialize

class FixedArenaController
{
public:
    template <int N>
    FixedArenaController(char(&a)[N]);

    ~FixedArenaController() = default;

    // no copy / no move
    FixedArenaController(const FixedArenaController&) = delete;
    FixedArenaController& operator=(const FixedArenaController&) = delete;
    FixedArenaController(FixedArenaController&&) noexcept = delete;
    FixedArenaController& operator=(FixedArenaController&&) noexcept = delete;

    void*  allocate(size_t);
    size_t block_size() const;
    size_t capacity() const;
    void   clear();
    bool   empty() const;
    void   dump() const;

private:
    void*  m_arena;
    size_t m_arenaSize;
    size_t m_blockSize;
};

template <int N>
inline FixedArenaController::FixedArenaController(char(&arena)[N])
    : m_arena{ arena }, m_arenaSize{ N }, m_blockSize{ 0 }
{
    std::println("FixedArenaController: N = {}", N);

    std::println("  Start of Arena: {:#X} ", reinterpret_cast<intptr_t>(m_arena));
}

inline void* FixedArenaController::allocate(size_t size) {

    if (!empty()) {
        return nullptr;           // arena already allocated
    }

    m_blockSize = std::max(size, sizeof(void*));
    size_t count = capacity();

    if (count == 0) {
        return nullptr;           // arena not big enough for even one item
    }

    char* p;
    for (p = (char*) m_arena; count > 1; --count, p += m_blockSize) {
        *reinterpret_cast<char**>(p) = p + m_blockSize;

        std::println("{:#X} => {:#X}", 
            reinterpret_cast<intptr_t>(p),
            reinterpret_cast<intptr_t>(p + m_blockSize)
        );
    }
    *reinterpret_cast<char**>(p) = nullptr;

    std::println("{:#X} => {:#X}",
        reinterpret_cast<intptr_t>(p),
        0
    );
    std::println();

    return m_arena;
}

inline size_t FixedArenaController::block_size() const {
    return m_blockSize;
}

inline size_t FixedArenaController::capacity() const {
    return m_blockSize ? (m_arenaSize / m_blockSize) : 0;
}

inline void FixedArenaController::clear() {
    m_blockSize = 0;
}

inline bool FixedArenaController::empty() const {
    return m_blockSize == 0;
}

inline void FixedArenaController::dump() const {

    std::println("Dump of Arena: ");
    std::println("    Total Size:  {}", m_arenaSize);
    std::println("    Block Size:  {}", m_blockSize);
    std::println("    Free Blocks: {}", "???");

    if (!empty()) {

        std::println("Dump of Arena: ");

        size_t n{};
        char* start = reinterpret_cast<char*>(m_arena);
        while (start != nullptr && *reinterpret_cast<char**>(start) != nullptr) {

            std::println("{}. Block: {:#X} ", n + 1, reinterpret_cast<intptr_t>(start));
            start += m_blockSize;
            ++n;
        }
    }
}

// ===========================================================================
// End-of-File
// ===========================================================================
