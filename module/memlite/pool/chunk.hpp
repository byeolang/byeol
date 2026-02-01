/// @file
#pragma once

#include "memlite/pool/allocator.hpp"

namespace by {

    /** @ingroup memlite
     *  @brief Memory chunk for block-based allocation
     *  @details The minimum unit class that can actually allocate memory in memlite. All
     *  memory management is performed by linking chunks together.
     *
     *  @remark Fixed memory size
     *  chunk has a _resize() function for flexible memory growth, but following memlite's
     *  concept, this is not exposed publicly. Consequently, chunk memory is fixed at object
     *  creation. If additional memory is needed, create and operate more chunk objects.
     *
     *  @section block_size Block size
     *  chunk receives blockSize and size at creation. blockSize is the minimum unit size
     *  one instance occupies in memory. size defines how many such instances can fit.
     *  For example, to create a chunk holding 100 int64 values:
     *  @code
     *      new chunk(sizeof(int64), 100)
     *  @endcode
     *
     *  @section real_block_size Real block size
     *  Actual memory allocation uses real block size instead of block size for optimization.
     *  Rather than allocating in small 1 or 2 byte units during CPU operations, it's more
     *  efficient to allocate minimum bytes appropriate for the CPU level, like 4 or 8 bytes.
     *
     *  @section arraylist_implementation ArrayList implementation
     *  chunk is implemented as an ArrayList. Size is fixed, but addition and deletion are
     *  free within size limits, with very fast random access.
     *
     *  Algorithm:
     *  0. Precondition: Each element's byte size must be >= 4. All elements have identical
     *     byte sizes. Since chunk handles everything as void*, elements without values are
     *     treated as int type.
     *  1. On array initialization with size, place integer value n+1 before the bytes
     *     occupied by nth element. Example: size=4 results in [1, 2, 3, 4]
     *  2. _head represents index of most recently added element, initialized to 0. _heap
     *     points to heap-allocated memory.
     *  3. When external new1 requests memory allocation, _head is assigned the int value at
     *     current _head-th element. Example: first new1() assigns _head the value `1` from
     *     _heap[0], meaning next new1() treats _heap[1] as available empty element.
     *  4. Return address of allocated memory for just-retrieved element.
     *     _head = 1, [in-use, 2, 3, 4]
     *  5. On memory release, receive memory address to release as void* parameter.
     *     Example: del(used = _heap[0])
     *  6. Assign _head value to that memory (assuming destructor already called).
     *     *used = _head // [1, 2, 3, 4]
     *  7. Assign _head to index of currently releasing memory. Calculate index via pointer
     *     arithmetic for _heap distance from _head.
     *     _head = _heap - used // _heap - _heap --> 0
     *     _head = 0, [1, 2, 3, 4]
     */
    class _nout chunk: public allocator {
        BY_ME(chunk, allocator)
        BY_INIT_META(me)
        friend class watcher;

    public:
        //  chunk:
        static constexpr ncnt MIN_SZ = 20;

    public:
        explicit chunk(ncnt blockSz = 1, ncnt sz = MIN_SZ);
        ~chunk();

    public:
        void* operator[](nidx n);
        const void* operator[](nidx n) const BY_CONST_FUNC(operator[](n));

    public:
        void* get(nidx n) { return _get(n); }

        //  allocator:
        void* new1() override;
        nbool del(void* used, ncnt) override;
        //  MemoryHaver:
        ncnt len() const override;
        ncnt size() const override;
        nbool rel() override;

        using super::has;
        /**
         * @brief Checks if this chunk contains the memory allocated for the given instance.
         * @param it The instance whose memory location is to be checked.
         * @return true if the instance's memory is within this chunk, false otherwise.
         */
        nbool has(const instance& it) const override;

    protected:
        /**
         *  @return returns regarding current size, not length.
         *          can return garbage if size is bigger than n.
         */
        void* _get(nidx n) override;
        nuchar* _getHeap();
        const nuchar* _getHeap() const BY_CONST_FUNC(_getHeap())
        nbool _resize(ncnt newSz);

    private:
        //  chunk:
        const nuchar* _getEOB() const BY_CONST_FUNC(_getEOB())
        nuchar* _getEOB();
        nbool _index(nidx start);
        ncnt _getRealBlkSize();
        void* _allocHeap(ncnt new_sz);
        nbool _freeHeap(nuchar** heap);

        nidx _head;
        ncnt _len, _sz;
        nuchar* _heap;
    };
} // namespace by
