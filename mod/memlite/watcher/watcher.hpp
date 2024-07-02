#pragma once

#include "../pool/chunk.hpp"
#include "watchCell.hpp"

namespace nm {

    class _nout watcher : public chunk {
        NM_ME(watcher, chunk)
        NM_INIT_META(me)
        friend class instancer;
        friend class bindTag;

    public:
        //  watcher:
        watcher();

    public:
        watchCell& operator[](nidx n);
        watchCell& operator[](id newId);
        const watchCell& operator[](nidx n) const NM_UNCONST_FUNC(operator[](n))
        const watchCell& operator[](id newId) const NM_UNCONST_FUNC(operator[](newId))

    public:
        watchCell& get(nidx n);
        watchCell& get(id newId);
        const watchCell& get(nidx n) const NM_UNCONST_FUNC(get(n));
        const watchCell& get(id newId) const NM_UNCONST_FUNC(get(newId));
        //  Allocator:
        void* new1() override;

        nbool del(void* used, ncnt sz) override;

    protected:
        //  watcher:
        id _genId(void* pt) const;
        nidx _getIdx(void* it) const;
    };
}
