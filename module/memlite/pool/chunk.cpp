#include "memlite/pool/chunk.hpp"

namespace by {

    BY_DEF_ME(chunk, allocator)

    me::chunk(ncnt blksize, ncnt sz): super(blksize), _head(0), _len(0), _sz(0), _heap(0) { _resize(sz); }

    me::~chunk() { me::rel(); }

    void* me::operator[](nidx n) { return get(n); }

    ncnt me::len() const { return _len; }

    ncnt me::size() const { return _sz; }

    nbool me::_resize(ncnt newSz) {
        if(newSz < MIN_SZ) newSz = MIN_SZ;
        WHEN(newSz == _sz) .ret(false);

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
        WHEN(_len >= _sz)
            .err(
                "new1() failed. chunk was full. you should have not called this in this "
                "situtation.")
                .ret(nullptr);

        nidx& ret = (nidx*) _get(_head) OR.ret(nullptr);
        _head = ret;
        _len++;
        return &ret;
    }

    nbool me::del(void* used, ncnt) {
        WHEN(!used) .ret(false);

        *(nidx*) used = _head;
        _head = ((nuchar*) used - _heap) / _getRealBlkSize();
        _len--;
        WHEN(_head < 0)
            .err("chunk corrupted! used(%s) apparently wasn't on heap(%s).", used, (void*) _heap).ret(false);
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
        WHEN(n < 0 || n >= size()) .ret(nullptr);

        return _heap + n * _getRealBlkSize();
    }

    nuchar* me::_getEOB() {
        nuchar& org = (nuchar*) _get(_sz - 1) OR.ret(nullptr);
        return &org + _getRealBlkSize() - 1;
    }

    nbool me::_index(nidx start) {
        for(ncnt n = start; n < _sz; n++)
            *(nidx*) _get(n) = n + 1;

        return true;
    }

    ncnt me::_getRealBlkSize() {
        ncnt sz = getBlkSize();
        constexpr ncnt bytes = buildFeature::platform::getArchBits() / 8;
        return sz < bytes ? bytes : sz;
    }

    void* me::_allocHeap(ncnt newSz) { return malloc(newSz * _getRealBlkSize()); }

    nbool me::_freeHeap(nuchar** heap) {
        if(*heap) {
            free(*heap);
            *heap = NULL;
        }
        return true;
    }
} // namespace by
