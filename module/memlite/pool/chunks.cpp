#include "memlite/pool/chunks.hpp"
#include "memlite/interface/instance.hpp"
#include "memlite/internal/memliteInternal.hpp"

namespace by {

    BY_DEF_ME(chunks, allocator)

    me::chunks(ncnt blkbyte): super(blkbyte), _s(0) {}

    me::~chunks() { _rel(); }

    chunk& me::operator[](nidx n) { return *get(n); }

    chunk& me::operator[](const instance& it) { return *get(it); }

    chunk* me::get(nidx n) { return (chunk*) _get(n); }

    chunk* me::get(const instance& it) { return (chunk*) _get(it.getId().chkN); }

    nbool me::rel() { return _rel(); }

    ncnt me::len() const { return _chunks.size(); }

    ncnt me::size() const { return len(); }

    void* me::new1() {
        nidx n = _findCapable();
        void* ret = _chunks[n]->new1();
        instance::getVault().set(ret, n);
        return ret;
    }

    nbool me::del(void* pt, ncnt sz) {
        //  in fact, cast wasn't be deallocated yet:
        //      if we guarrantee that destructor didn't change its _id value,
        //      _id will keep its value till now.
        nidx chkN = memliteInternal::getId(*(instance*) pt).chkN;
        return _chunks[chkN]->del(pt, sz);
    }

    nidx me::_findCapable() {
        ncnt sz = _chunks.size();
        nidx end = _s;

        do {
            chunk* e = get(_s);
            WHEN(e && e->isCapable()) .ret(_s);
            _s++;
            if(_s >= sz) _s = 0;
        } while(_s != end);

        resize(len() + 1);
        return _s;
    }

    nbool me::has(const instance& it) const { return _chunks[it.getId().chkN]->has(it); }

    nbool me::resize(ncnt new1) {
        _s = _chunks.size();
        if(_s > new1) _s = new1 - 1;
        if(_s < 0) _s = 0;

        while(_chunks.size() < new1)
            _chunks.push_back(new chunk(getBlkSize()));
        return true;
    }

    nbool me::_rel() {
        for(chunk* e: _chunks)
            e->rel();
        _chunks.clear();
        _s = 0;
        return true;
    }

    void* me::_get(nidx n) {
        WHEN(n < 0 || n >= len()) .ret(nullptr);

        return _chunks[n];
    }
} // namespace by
