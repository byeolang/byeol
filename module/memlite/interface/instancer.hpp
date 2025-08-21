/// @file
#pragma once

#include "memlite/pool/pool.hpp"
#include "memlite/watcher/watcher.hpp"

namespace by {

    class _nout instancer {
        BY_ME(instancer)
        BY_INIT_META(me)
        friend class instance;

    public:
        ~instancer();

    public:
        nbool bind(const instance& it);
        nbool bind(const instance* it) BY_SIDE_FUNC(bind);
        nbool rel(const instance& old);
        nbool rel(const instance* it) BY_SIDE_FUNC(rel);
        const pool& getPool() const;
        const watcher& getWatcher() const;
        /// @return singleton instance. this returns nul of reference if app is about to be
        /// terminated.
        static me* get();

    private:
        void* _new1(size_t sz);
        void _del(void* pt, ncnt sz);
        nbool _hasBindTag(const instance& it) const;

    private:
        instancer() = default;

    private:
        static nbool _isRel;
        pool _pool;
        watcher _watcher;
    };
} // namespace by
