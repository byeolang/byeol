#include "instancer.hpp"

namespace nm {

    NM_DEF_ME(instancer)

    nbool me::bind(const instance& new1) {
        watchCell& un = *(watchCell*) _watcher.new1();
        return un.blk.bind(new1);
    }

    nbool me::rel(const instance& old) {
        if(!_hasBindTag(old)) return true; // optimization.

        watchCell& un = _watcher[old.getId()];
        if(nul(un)) return false;
        return _watcher.del(&un, sizeof(watchCell));
    }

    void* me::_new1(size_t sz) {
        return _pool[sz].new1();
    }

    void me::_del(void* pt, ncnt sz) {
        _pool[sz].del(pt, sz);
    }

    const pool& me::getPool() const {
        return _pool;
    }

    const watcher& me::getWatcher() const {
        return _watcher;
    }

    me& me::get() {
        static me inner;
        return inner;
    }

    nbool me::_hasBindTag(const instance& it) const {
        return it._id.tagN != NM_INDEX_ERROR;
    }
}
