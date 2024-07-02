#pragma once

#include "../pool/pool.hpp"
#include "../watcher/watcher.hpp"

namespace nm {

    class _nout instancer {
        NM_ME(instancer)
        NM_INIT_META(me)
        friend class instance;

    public:
        nbool bind(const instance& new1);
        nbool rel(const instance& old);
        const pool& getPool() const;
        const watcher& getWatcher() const;
        static NM_SINGLETON_GETTER(me)

    private:
        void* _new1(size_t sz);
        void _del(void* pt, ncnt sz);
        nbool _hasBindTag(const instance& it) const;


    private:
        pool _pool;
        watcher _watcher;
    };
}
