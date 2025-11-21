/// @file
#pragma once

#include "memlite/pool/chunk.hpp"
#include "memlite/watcher/bindTag.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Memory watcher for tracking instance lifecycle and allocation
    /// @details Monitors memory allocation and deallocation, providing
    /// watch cells for tracking instance states and managing memory lifecycle.
    class _nout watcher: public chunk {
        BY_ME(watcher, chunk)
        BY_INIT_META(me)
        friend class instancer;
        friend class bindTag;

    public:
        //  watcher:
        watcher();

    public:
        bindTag& operator[](nidx n);
        const bindTag& operator[](nidx n) const BY_CONST_FUNC(operator[](n));
        bindTag& operator[](id id);
        const bindTag& operator[](id id) const BY_CONST_FUNC(operator[](id));

    public:
        bindTag* get(nidx n);
        bindTag* get(id newId);
        const bindTag* get(nidx n) const BY_CONST_FUNC(get(n))
        const bindTag* get(id newId) const BY_CONST_FUNC(get(newId))
        //  Allocator:
        void* new1() override;

        nbool del(void* used, ncnt sz) override;

    protected:
        //  watcher:
        id _genId(void* pt) const;
        nidx _getIdx(void* it) const;
    };
} // namespace by
