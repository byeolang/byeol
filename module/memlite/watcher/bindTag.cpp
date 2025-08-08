#include "memlite/watcher/bindTag.hpp"

#include "memlite/binder/tbindable.inl"
#include "memlite/interface/instancer.hpp"

namespace by {

    BY_DEF_ME(bindTag)

    me::bindTag(): _pt(NULL), _strong(0) {}

    me::bindTag(id newId): _pt(NULL), _strong(0), _id(newId) {}

    me::~bindTag() { _id.serial = 0; }

    const chunk* me::getChunk() const {
        WHEN(!_pt).ret(nullptr);

        return instancer::get() TO(getPool().get(*_pt)->get(*_pt));
    }

    ncnt me::getStrongCnt() const { return _strong; }

    void me::rel() {
        if(_pt && _id.isHeap()) delete _pt;

        _pt = nullptr;
        _strong = 0;
    }

    nbool me::isBind() const { return _pt; }

    const type& me::getBindable() const { return ttype<instance>::get(); }

    nbool me::canBind(const type& type) const { return type.isSub(getBindable()); }

    id me::getId() const { return _id; }

    nbool me::_onStrong(ncnt vote) {
        WHEN(vote < 0 && _strong <= 0).ret(false);
        WHEN(!_id.isHeap()).ret(false);

        _strong += vote;
        if(_strong <= 0) rel();
        return true;
    }

    instance* me::get() { return _pt; }

    nbool me::bind(const instance& it) {
        rel();

        _pt = (instance*) &it;
        return _completeId(*_pt);
    }

    nbool me::_completeId(instance& it) {
        //  complete mine:
        id mine = getId();
        mine.chkN = it._id.chkN;
        //  propagate it:
        return _sync(mine);
    }

    nbool me::_sync(id new1) {
        if(_pt) _pt->_setId(new1);
        _id = new1;
        return true;
    }

    const bindTag* me::getBindTag(id newId) {
        const watchCell& cell = instancer::get() TO(getWatcher().get(newId)) OR.ret(nullptr);
        return &cell.blk;
    }

    instance* me::operator->() { return get(); }

    instance& me::operator*() { return *get(); }

    const type& me::getType() const { return ttype<bindTag>::get(); }
} // namespace by
