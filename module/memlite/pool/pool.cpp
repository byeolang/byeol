#include "memlite/pool/pool.hpp"
#include "memlite/interface/instance.hpp"

namespace by {

    NM_DEF_ME(pool, memoryHaver)

    me::pool() {}

    me::~pool() { rel(); }

    chunks* me::get(const instance& it) { return get(it.getType().size()); }

    chunks* me::get(nidx n) { return (chunks*) _get(n); }

    nbool me::has(const instance& it) const {
        const chunks& got = get(it.getType().size()) OR.warn("got == null").ret(false);
        return got.has(it);
    }

    ncnt me::size() const { return _chunks.capacity(); }

    ncnt me::len() const { return _chunks.size(); }

    nbool me::rel() {
        for(chunks* e: _chunks) {
            if(!e) continue;
            e->rel();
            delete e;
        }

        _chunks.clear();
        return true;
    }

    void* me::_get(nidx n) {
        while((int) _chunks.size() - 1 < n)
            _chunks.push_back(NULL);

        if(!_chunks[n]) _chunks[n] = new chunks(n);

        return _chunks[n];
    }
} // namespace by
