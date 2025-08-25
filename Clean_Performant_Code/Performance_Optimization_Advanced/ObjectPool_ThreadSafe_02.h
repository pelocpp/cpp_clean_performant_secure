// ===========================================================================
// ObjectPool_ThreadSafe_02.h // Performance Optimization Advanced
// From: https://radiantsoftware.hashnode.dev/c-lock-free-object-pool
// Status: Not yet prepared // To be Done
// ===========================================================================

#pragma once

#include <atomic>
#include <new>
#include <utility>

//Alignas so can be used naturally in std::atomic
class alignas(2 * alignof(int)) GuardedIndex
{
public:
    constexpr GuardedIndex() = default;
    constexpr GuardedIndex(int aIndex) : mIndex(aIndex) {}

    constexpr int Get_Index() const { return mIndex; }
    constexpr void Set_Index(int aIndex) { mIndex = aIndex; ++mGuardCount; }

private:
    int mIndex = 0;
    unsigned int mGuardCount = 0; //Unsigned so rollover is defined
};

static_assert(std::atomic<GuardedIndex>::is_always_lock_free,
    "This will be much slower! Consider reducing the size of the members!");

template <typename DataType, bool Awaitable = false>
class Pool
{
private:
    //Get the cache line size (typically 64 bytes)
    static constexpr auto sMemberAlign = std::hardware_destructive_interference_size;

    //Modified frequently during operations:
    alignas(sMemberAlign) std::atomic<int> mSize = 0;
    alignas(sMemberAlign) GuardedIndex mHeadNodeIndex;

    //Not modified post-allocation:
    alignas(sMemberAlign) std::byte* mStorage = nullptr; //Object memory
    int* mFreeList = nullptr;
    int mCapacity = 0;

public:
    template <typename AllocatorType>
    void allocate(AllocatorType& aAllocator, int aCapacity);

    template <typename AllocatorType>
    void free(AllocatorType& aAllocator);

    template <typename... ArgumentTypes>
    std::pair<bool, int> emplace(ArgumentTypes&&... aArguments);

    template <typename... ArgumentTypes>
    //  requires (Awaitable)
    int emplaceAwait(ArgumentTypes&&... aArguments);

    DataType& operator[](int aIndex);
    const DataType& operator[](int aIndex) const;

    void erase(int aIndex);

    void decreaseSize();

    int size() const;
    bool empty() const;
};


template <typename DataType, bool Awaitable >
template <typename AllocatorType>
inline void Pool<DataType, Awaitable>::allocate(AllocatorType& aAllocator, int aCapacity)
{
    DataType* ptr = nullptr;

    //Allocate the object memory
    // Original
    // static constexpr auto sDataAlign = std::max(alignof(DataType), sMemberAlign);


    static auto sDataAlign = std::max(alignof(DataType), sMemberAlign);



    // Original
    // mStorage = aAllocator.allocate(aCapacity * sizeof(DataType), sDataAlign);

    // adpopted
    auto size = aCapacity * sizeof(DataType);
    // auto* memory = aAllocator.allocate(aCapacity * sizeof(DataType));  // Da ist das Alignmenet Thema offen // original
    // 
    
    
    
    // WEITER: Ist nicht übersetzungsfähig
    //auto* memory = aAllocator.allocate(size);
    //mStorage = static_cast<DataType>(memory);




    mCapacity = aCapacity;

    // allocate the free list memory
    static constexpr auto sListAlign = std::max(alignof(int), sMemberAlign);

    // Original
    //auto cFreeListMemory = aAllocator.allocate(aCapacity * sizeof(int), sListAlign);
    //mFreeList = reinterpret_cast<int*>(cFreeListMemory);

    auto cFreeListMemory = aAllocator.allocate(aCapacity * sizeof(int));
    mFreeList = reinterpret_cast<int*>(cFreeListMemory);



    //Initialize free list  // ????????????????????
    //for (int ci = 0; ci < (aCapacity - 1); ++ci)
    //    new (&mFreeList[ci]) = int(ci + 1);
    //new (&mFreeList[aCapacity - 1]) = int(-1);

    for (int ci = 0; ci < (aCapacity - 1); ++ci)
        new (&mFreeList[ci]) int(ci + 1);
    new (&mFreeList[aCapacity - 1])  int(-1);

    //Publish free list head node. 
    mHeadNodeIndex.Set_Index(0);
}

template <typename DataType, bool Awaitable >
template <typename AllocatorType>
inline void Pool<DataType, Awaitable>::free(AllocatorType& aAllocator)
{
    Assert(empty(), "Objects not destroyed!\n");

    //Note: no destruction of indices are needed: is trivial type
    aAllocator.Free(reinterpret_cast<std::byte*>(mFreeList));
    mFreeList = nullptr;
    aAllocator.Free(mStorage);
    mStorage = nullptr;

    mCapacity = 0;
}

template <typename DataType, bool Awaitable >
inline int Pool<DataType, Awaitable>::size() const
{
    return mSize.load(std::memory_order::relaxed);
}

template <typename DataType, bool Awaitable >
inline bool Pool<DataType, Awaitable>::empty() const
{
    return (size() == 0);
}

template <typename DataType, bool Awaitable>
template <typename... ArgumentTypes>
inline std::pair<bool, int> Pool<DataType, Awaitable>::emplace(ArgumentTypes&&... aArguments)
{
    //Get initial value
    auto cHead = std::atomic_ref(mHeadNodeIndex);
    GuardedIndex cHeadEntry = cHead.load(std::memory_order::acquire);

    int cFirstSlot{}; // Added PeLo // WELCHER WERT ???????????????????????????

    //Loop until we reserve the head node for our object
    int cHeadIndex;
    GuardedIndex cNextSlotEntry;
    do
    {
        //Bail if we're full
        cHeadIndex = cHeadEntry.Get_Index();
        if (cHeadIndex == -1)
            return { false, cFirstSlot };

        //Find the next slot in the free list
        auto cListNode = std::atomic_ref(mFreeList[cFirstSlot]);
        auto cNextSlotIndex = cListNode.load(std::memory_order::relaxed);

        //Set next index and guard against ABA
        cNextSlotEntry = cHeadEntry;
        cNextSlotEntry.Set_Index(cNextSlotIndex);

        //Set new value for head index
    } while (!cHead.compare_exchange_weak(cHeadEntry, cNextSlotEntry,
        std::memory_order::acquire, std::memory_order::acquire));

    //We have exclusive access to this slot. Create new object
    std::byte* cAddress = mStorage + cFirstSlot * sizeof(DataType);
    new (cAddress) DataType(std::forward<ArgumentTypes>(aArguments)...);

    //Update the size and return
    mSize.fetch_add(1, std::memory_order::relaxed);
    return { true, cHeadIndex };
}

template <typename DataType, bool Awaitable>
inline DataType& Pool<DataType, Awaitable>::operator[](int aIndex)
{
    //Non-const method calls the const method
    const auto& cConstThis = *const_cast<const Pool*>(this);
    return const_cast<DataType&>(cConstThis[aIndex]);
}

template <typename DataType, bool Awaitable>
inline const DataType& Pool<DataType, Awaitable>::operator[](int aIndex) const
{
    const std::byte* cAddress = mStorage + aIndex * sizeof(DataType);
    return *std::launder(reinterpret_cast<const DataType*>(cAddress));
}

template <typename DataType, bool Awaitable>
inline void Pool<DataType, Awaitable>::erase(int aIndex)
{
    //Destroy the object
    (*this)[aIndex].~DataType();

    //Add this index to the front of the list
    auto cHead = std::atomic_ref(mHeadNodeIndex);
    GuardedIndex cHeadEntry = cHead.load(std::memory_order::relaxed);
    GuardedIndex cNewHeadEntry;
    do
    {
        //Store the current head node in our free list slot
        auto cListNode = std::atomic_ref(mFreeList[aIndex]);
        cListNode.store(cHeadEntry.Get_Index(), std::memory_order::relaxed);

        //Set our index and guard against ABA
        cNewHeadEntry = cHeadEntry;
        cNewHeadEntry.Set_Index(aIndex);

        //Set new value
    } while (!cHead.compare_exchange_weak(cHeadEntry, cNewHeadEntry,
        std::memory_order::release, std::memory_order::relaxed));

    //Decrease the size
    decreaseSize();
}

template <typename DataType, bool Awaitable>
template <typename... ArgumentTypes>
inline int Pool<DataType, Awaitable>::emplaceAwait(ArgumentTypes&&... aArguments)
  //requires (Awaitable)
{
    while (true)
    {
        auto [cCreated, cIndex] =
            Emplace(std::forward<ArgumentTypes>(aArguments)...);

        if (cCreated)
            return cIndex;

        //The pool was full, wait until we're notified
        mSize.wait(mCapacity, std::memory_order::acquire);
    }
}

template <typename DataType, bool Awaitable>
inline void Pool<DataType, Awaitable>::decreaseSize()
{
    //Release if awaiting (Syncs indices), else relaxed (nothing to sync)
    static constexpr auto sOrder = Awaitable ? std::memory_order::release : std::memory_order::relaxed;
    [[maybe_unused]] auto cPriorSize = mSize.fetch_sub(1, sOrder);

    if constexpr (Awaitable)
    {
        if (cPriorSize == mCapacity)
            mSize.notify_all();
    }
}

// ===========================================================================
// End-of-File
// ===========================================================================
