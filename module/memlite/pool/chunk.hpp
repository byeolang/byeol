/// @file
#pragma once

#include "memlite/pool/allocator.hpp"

namespace by {

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
        nbool has(const instance& it) const override;

    protected:
        //  MemoryHaver:
        /// @return returns regarding current size, not length.
        ///         can return garbage if size is bigger than n.
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
