#include "instance.hpp"

#include <iostream>
#include <vector>

#include "instancer.hpp"

namespace nm {

    NM_DEF_ME(instance)
    me::vault instance::_vault;

    me::instance() { _id.chkN = _vault.get(this); }

    me::instance(id newId): _id(newId) {} // no binding required.

    me::instance(const me& rhs) {
        _id.chkN = _vault.get(this); // _id is only belonged to the instance. not able to be copied.
    }

    me::~instance() { _getMgr().rel(*this); }

    ncnt me::vault::len() const { return _vaults.size(); }

    std::map<void*, int>& me::vault::getVaults() { return _vaults; }

    void* me::operator new(size_t sz) noexcept { return _getMgr()._new1(sz); }

    void me::operator delete(void* pt, size_t sz) noexcept { _getMgr()._del(pt, sz); }

    id me::getId() const {
        if(_id.tagN == NM_INDEX_ERROR) _getMgr().bind((me&) *this);
        return _id;
    }

    nbool me::isHeap() const { return _id.isHeap(); }

    const bindTag& me::getBindTag() const { return bindTag::getBindTag(getId()); }

    nbool me::_setId(id new1) {
        // rel() must not to reset Id. it's regarding to instance info.
        // as long as instance keep alive, that info need to be stuck to instance.
        _id = new1;
        return true;
    }

    instancer& me::_getMgr() { return instancer::get(); }

    void me::vault::set(void* ptr, nidx chkN) {
        WHEN_NUL(ptr).ret();

        _vaults[ptr] = chkN;
    }

    nidx me::vault::get(void* ptr) {
        auto e = _vaults.find(ptr);
        nidx ret = e == _vaults.end() ? NM_INDEX_ERROR : _vaults[ptr];
        if(ret > NM_INDEX_ERROR) _vaults.erase(ptr);
        return ret;
    }

    nbool me::vault::rel() {
        _vaults.clear();
        return true;
    }
} // namespace nm
