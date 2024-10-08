#include "chunk.hpp"

namespace nm {

    NM_DEF_ME(chunk, allocator)

    me::chunk(ncnt blksize, ncnt sz): super(blksize), _head(0), _len(0), _sz(0), _heap(0) {
        _resize(sz);
    }

    me::~chunk() { me::rel(); }

    ncnt me::len() const { return _len; }

    ncnt me::size() const { return _sz; }

    nbool me::_resize(ncnt newSz) {
        if(newSz < MIN_SZ) newSz = MIN_SZ;
        if(newSz == _sz) return false;

        nuchar* new1 = (nuchar*) _allocHeap(newSz);
        // considered if user resize far smaller rather than what it had.
        if(_heap) {
            ncnt min = _sz < newSz ? _sz : newSz;
            memcpy(new1, _heap, min * _getRealBlkSize());
        }

        //  post:
        _freeHeap(&_heap);
        _heap = new1;
        _sz = newSz;
        return _index(_len);
    }

    void* me::new1() {
        if(_len >= _sz)
            return NM_E(
                       "new1() failed. chunk was full. you should have not called this in this "
                       "situtation."),
                   nullptr;

        nidx* ret = (nidx*) _get(_head) orRet nullptr;
        _head = *ret;
        _len++;
        return ret;
    }

    nbool me::del(void* used, ncnt) {
        if(!used) return false;

        *(nidx*) used = _head;
        _head = ((nuchar*) used - _heap) / _getRealBlkSize();
        _len--;
        if(_head < 0)
            return NM_E("chunk corrupted! used(%s) apparently wasn't on heap(%s).", used,
                       (void*) _heap),
                   false;
        return true;
    }

    nbool me::rel() {
        _len = _sz = 0;
        _head = 0;
        return _freeHeap(&_heap);
    }

    nbool me::has(const instance& it) const {
        void* pt = (void*) &it;
        return _heap && _heap <= pt && pt <= _getEOB();
    }

    nuchar* me::_getHeap() { return _heap; }

    void* me::_get(nidx n) {
        if(n < 0 || n >= size()) return nullptr;

        return _heap + n * _getRealBlkSize();
    }

    nuchar* me::_getEOB() {
        nuchar* org = (nuchar*) _get(_sz - 1) orRet nullptr;
        return org + _getRealBlkSize() - 1;
    }

    nbool me::_index(nidx start) {
        for(ncnt n = start; n < _sz; n++)
            *(nidx*) _get(n) = n + 1;

        return true;
    }

    ncnt me::_getRealBlkSize() {
        ncnt sz = getBlkSize();
        return sz < 4 ? 4 : sz;
    }

    void* me::_allocHeap(ncnt newSz) { return malloc(newSz * _getRealBlkSize()); }

    nbool me::_freeHeap(nuchar** heap) {
        if(*heap) {
            free(*heap);
            *heap = NULL;
        }
        return true;
    }
} // namespace nm
