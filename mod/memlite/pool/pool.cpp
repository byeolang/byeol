#include "pool.hpp"

#include "../interface/instance.hpp"

namespace nm {

    NM_DEF_ME(pool, memoryHaver)

    me::pool() {}

    me::~pool() { rel(); }

    chunks& me::operator[](nidx n) { return get(n); }

    chunks& me::operator[](const instance& inst) { return get(inst); }

    chunks& me::get(const instance& inst) { return get(inst.getType().size()); }

    chunks& me::get(nidx n) { return *(chunks*) _get(n); }

    nbool me::has(const instance& it) const {
        const chunks &got = get(it.getType().size()) orRet1 NM_W("got == null"), false;
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
} // namespace nm
