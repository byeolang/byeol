/// @file
#pragma once

#include "memlite/pool/chunk.hpp"
#include "memlite/watcher/life.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Memory watcher for tracking instance lifecycle and allocation
    /// @details Monitors memory allocation and deallocation, providing
    /// watch lives for tracking instance states and managing memory lifecycle.
    class _nout watcher: public chunk {
        BY_ME(watcher, chunk)
        BY_INIT_META(me)
        friend class instancer;
        friend class life;

    public:
        //  watcher:
        watcher();

    public:
        life& operator[](nidx n);
        const life& operator[](nidx n) const BY_CONST_FUNC(operator[](n));
        life& operator[](id id);
        const life& operator[](id id) const BY_CONST_FUNC(operator[](id));

    public:
        life* get(nidx n);
        life* get(id newId);
        const life* get(nidx n) const BY_CONST_FUNC(get(n))
        const life* get(id newId) const BY_CONST_FUNC(get(newId))
        //  Allocator:
        void* new1() override;

        nbool del(void* used, ncnt sz) override;

    protected:
        //  watcher:
        id _genId(void* pt) const;
        nidx _getIdx(void* it) const;
    };
} // namespace by
