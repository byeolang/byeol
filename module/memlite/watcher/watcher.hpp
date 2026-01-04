/// @file
#pragma once

#include "memlite/pool/chunk.hpp"
#include "memlite/watcher/life.hpp"

namespace by {

    /// @ingroup memlite
    /// @brief Memory watcher for tracking instance lifecycle and allocation
    /// @details One pillar of memory management that manages created object lifecycles.
    /// Whenever an @ref instance is created, watcher additionally allocates a @ref life
    /// object and uses reference counting to determine object destruction timing.
    ///
    /// @section Reference counting
    /// When @ref binder binds an instance, life increments its strong count by 1. When
    /// binder calls rel() on instance, decrements count by 1. At 0, deletes from memory.
    /// instance uses `operator delete()` to make @ref instancer execute memory release work.
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
