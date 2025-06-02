#pragma once

#include "memlite/pool/pool.hpp"
#include "memlite/watcher/watcher.hpp"

namespace nm {

    class _nout instancer {
        NM_ME(instancer)
        NM_INIT_META(me)
        friend class instance;

    public:
        ~instancer();

    public:
        nbool bind(const instance& it);
        nbool bind(const instance* it) NM_SIDE_FUNC(bind, false) nbool rel(const instance& old);
        nbool rel(const instance* it) NM_SIDE_FUNC(rel, false) const pool& getPool() const;
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
}
