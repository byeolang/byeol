#include "memlite/interface/instance.hpp"

#include <iostream>
#include <vector>

#include "memlite/interface/instancer.hpp"
#include "memlite/internal/memliteInternal.hpp"

namespace by {

    BY_DEF_ME(instance)

    me::instance() { _id.chkN = getVault().get(this); }

    me::instance(id newId): _id(newId) {} // no binding required.

    me::instance(const me& rhs) {
        _id.chkN = getVault().get(this); // _id is only belonged to the instance. not able to be copied.
    }

    me::~instance() { _getMgr() TO(rel(*this)); }

    ncnt me::vault::len() const { return _vaults.size(); }

    std::map<void*, int>& me::vault::getVaults() { return _vaults; }

    void* me::operator new(size_t sz) noexcept { return memliteInternal::new1(sz); }

    void me::operator delete(void* pt, size_t sz) noexcept { memliteInternal::del(pt, sz); }

    id me::getId() const {
        if(_id.tagN == BY_INDEX_ERROR) _getMgr() TO(bind((me&) *this));
        return _id;
    }

    nbool me::isHeap() const { return _id.isHeap(); }

    const life* me::getBindTag() const { return life::getBindTag(getId()); }

    me::vault me::_vault;

    me::vault& me::getVault() { return _vault; }

    nbool me::_setId(id new1) {
        // rel() must not to reset Id. it's regarding to instance info.
        // as long as instance keep alive, that info need to be stuck to instance.
        _id = new1;
        return true;
    }

    instancer* me::_getMgr() { return instancer::get(); }

    void me::vault::set(void* ptr, nidx chkN) {
        WHEN_NUL(ptr).ret();

        _vaults[ptr] = chkN;
    }

    nidx me::vault::get(void* ptr) {
        auto e = _vaults.find(ptr);
        nidx ret = e == _vaults.end() ? BY_INDEX_ERROR : _vaults[ptr];
        if(ret > BY_INDEX_ERROR) _vaults.erase(ptr);
        return ret;
    }

    nbool me::vault::rel() {
        _vaults.clear();
        return true;
    }
} // namespace by
