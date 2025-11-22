#include "memlite/interface/instancer.hpp"

namespace by {

    BY_DEF_ME(instancer)

    nbool me::_isRel = false;

    me::~instancer() { _isRel = true; }

    nbool me::bind(const instance& it) {
        life& un = *(life*) _watcher.new1();
        return un.bind(it);
    }

    nbool me::rel(const instance& old) {
        WHEN(!_hasBindTag(old)) .ret(true); // optimization.

        life& un = _watcher.get(old.getId()) OR.ret(false);
        return _watcher.del(&un, sizeof(life));
    }

    void* me::_new1(size_t sz) { return _pool.get(sz) TO(new1()); }

    void me::_del(void* pt, ncnt sz) { _pool.get(sz)->del(pt, sz); }

    const pool& me::getPool() const { return _pool; }

    const watcher& me::getWatcher() const { return _watcher; }

    me* me::get() {
        static me inner;
        return _isRel ? nullptr : &inner;
    }

    nbool me::_hasBindTag(const instance& it) const { return it._id.tagN != BY_INDEX_ERROR; }
} // namespace by
