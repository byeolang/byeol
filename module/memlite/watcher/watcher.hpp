#pragma once

#include "memlite/pool/chunk.hpp"
#include "memlite/watcher/watchCell.hpp"

namespace by {

    class _nout watcher: public chunk {
        BY_ME(watcher, chunk)
        BY_INIT_META(me)
        friend class instancer;
        friend class bindTag;

    public:
        //  watcher:
        watcher();

    public:
        watchCell& operator[](nidx n);
        const watchCell& operator[](nidx n) const BY_CONST_FUNC(operator[](n));
        watchCell& operator[](id id);
        const watchCell& operator[](id id) const BY_CONST_FUNC(operator[](id));

    public:
        watchCell* get(nidx n);
        watchCell* get(id newId);
        const watchCell* get(nidx n) const BY_CONST_FUNC(get(n))
        const watchCell* get(id newId) const BY_CONST_FUNC(get(newId))
        //  Allocator:
        void* new1() override;

        nbool del(void* used, ncnt sz) override;

    protected:
        //  watcher:
        id _genId(void* pt) const;
        nidx _getIdx(void* it) const;
    };
} // namespace by
