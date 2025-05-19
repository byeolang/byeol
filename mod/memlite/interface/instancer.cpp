#include "memlite/interface/instancer.hpp"

namespace nm {

    NM_DEF_ME(instancer)

    nbool me::_isRel = false;

    me::~instancer() { _isRel = true; }

    nbool me::bind(const instance& new1) {
        watchCell& un = *(watchCell*) _watcher.new1();
        return un.blk.bind(new1);
    }

    nbool me::rel(const instance& old) {
        WHEN(!_hasBindTag(old)).ret(true); // optimization.

        watchCell& un = _watcher.get(old.getId()) OR.ret(false);
        return _watcher.del(&un, sizeof(watchCell));
    }

    void* me::_new1(size_t sz) {
        auto* got = _pool.get(sz);
        return got TO(new1());
        //return _pool.get(sz) TO(new1());
    }

    void me::_del(void* pt, ncnt sz) { _pool.get(sz)->del(pt, sz); }

    const pool& me::getPool() const { return _pool; }

    const watcher& me::getWatcher() const { return _watcher; }

    me* me::get() {
        static me inner;
        return _isRel ? nullptr : &inner;
    }

    nbool me::_hasBindTag(const instance& it) const { return it._id.tagN != NM_INDEX_ERROR; }
} // namespace nm
