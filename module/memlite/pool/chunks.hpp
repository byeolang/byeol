/// @file
#pragma once

#include "memlite/pool/chunk.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Collection of memory chunks for scalable allocation
    /// @details Manages multiple @ref chunk instances. Since chunk uses only fixed-size
    /// memory created at initialization, chunks adds/removes multiple chunks to manage
    /// memory dynamically.
    ///
    /// @section chunks also provides only fixed memory
    /// Since chunks adds or removes chunks, and each chunk cell uses only fixed size,
    /// chunks can also only allocate fixed-size memory.
    ///
    /// @section Coordination with pool
    /// The first to receive memory requests is @ref pool class. When pool receives a memory
    /// size to allocate, it lazily retrieves a chunks instance capable of handling that
    /// memory size and requests new1(). chunks::new1() finds a chunk capable of allocating
    /// memory, and if none exists, creates an additional chunk.
    ///
    /// @section Available chunk search algorithm
    /// The most recently memory-allocated chunk has the highest probability of being able
    /// to allocate additionally. Member variable `_s` holds the index of the most recently
    /// allocated chunk. If _chunks[_s] has no available memory, increment _s. Like a
    /// circular array, _chunks' end connects to its beginning. If _s loops back to its
    /// pre-traversal value and still no available memory exists, the entire chunks has no
    /// available memory, so enters resize().
    ///
    /// @remark Cannot use vector
    /// Obviously, vector is managed on heap, so shouldn't use vector while making a custom
    /// memory pool. Planned for future modification.
    class _nout chunks: public allocator {
        BY_ME(chunks, allocator)
        BY_INIT_META(me)

    public:
        //  chunks:
        explicit chunks(ncnt blkbyte = 0);
        virtual ~chunks();

    public:
        chunk& operator[](nidx n);
        const chunk& operator[](nidx n) const BY_CONST_FUNC(operator[](n));
        chunk& operator[](const instance& it);
        const chunk& operator[](const instance& it) const BY_CONST_FUNC(operator[](it));

    public:
        //  chunks:
        chunk* get(nidx n);
        chunk* get(const instance& it);
        chunk* get(const instance* it) BY_SIDE_FUNC(get);
        const chunk* get(nidx n) const BY_CONST_FUNC(get(n))
        const chunk* get(const instance& it) const BY_CONST_FUNC(get(it))
        const chunk* get(const instance* it) const BY_CONST_FUNC(get(it))

        //  allocator:
        void* new1() override;
        nbool del(void* pt, ncnt sz) override;
        /// @remark @ref chunk can resize its data. but can't persist whole memory allocated before,
        ///         it's a kind of memory flashing and can't give a way for accessing it.
        ///         at outside, ptr for them should be daggled.
        virtual nbool resize(ncnt new1);

        using super::has;
        nbool has(const instance& it) const override;

        ncnt len() const override;
        ncnt size() const override;
        nbool rel() override;

    private:
        //  chunks:
        nidx _findCapable();
        nbool _rel();
        //  MemoryHaver:
        void* _get(nidx n) override;

    private:
        std::vector<chunk*> _chunks;
        int _s;
    };
} // namespace by
