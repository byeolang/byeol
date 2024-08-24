#include "origin.hpp"

#include "../type/mgdType.hpp"
#include "exprs/runExpr.hpp"
#include "frame/frameInteract.hpp"
#include "../builtin/err/baseErr.hpp"
#include "../frame/thread.hpp"

namespace nm {

    NM(DEF_ME(origin))

    me::origin(const mgdType& newType): super(), _type(newType), _state(RELEASED) {
        _setOrigin(*this);
    }

    me::origin(const mgdType& newType, scope& shares, scope& owns):
        super(shares, owns), _type(newType), _state(RELEASED) {
        _setOrigin(*this);
    }

    me::origin(const mgdType& newType, const obj& subpack):
        super(), _type(newType), _subpack(subpack), _state(RELEASED) {
        _setOrigin(*this);
    }

    me::origin(const me& rhs):
        super(rhs),
        _type(rhs._type),
        _subpack(rhs._subpack),
        _src(rhs._src),
        _callComplete(rhs._callComplete),
        _state(rhs._state) {
        // usually all obj called by copyctor is complete object.
        // but, origin obj should not.
    }

    me& me::operator=(const me& rhs) {
        if(this == &rhs) return *this;

        super::operator=(rhs);
        return _assign(rhs);
    }

    scope& me::subs() {
        if(_state == VERIFIED && !thread::get().getFrames().isEmpty()) _runCallComplete();

        return super::subs();
    }

    state me::getState() const { return _state; }

    void me::setState(state new1) { _state = new1; }

    const ntype& me::getType() const { return _type; }

    const obj& me::getSubPack() const { return *_subpack; }

    const src& me::getSrc() const { return *_src; }

    clonable* me::cloneDeep() const {
        // update origin:
        //  this makes an origin object. and cloned origin object's origin should itself.
        //  but don't bind _org to this. it's circular dependency.
        me* ret = new me(*this);
        ret->subs().link(*(scope*) getShares().cloneDeep());
        return ret;
    }

    const baseObj& me::getOrigin() const { return *this; }

    baseObj* me::make() const { return new obj(*this); }

    runExpr& me::getCallComplete() { return *_callComplete; }

    void me::setCallComplete(const runExpr& new1) { _callComplete.bind(new1); }

    nbool me::isComplete() const { return _callComplete.isBind(); }

    clonable* me::clone() const { return new obj(*this); }

    void me::_setType(const mgdType& new1) { _type = new1; }

    void me::_setSubPack(const obj& subpack) { _subpack.bind(subpack); }

    void me::_setSrc(const src& s) { _src.bind(s); }

    me& me::_assign(const me& rhs) {
        _type = rhs._type;
        _subpack = rhs._subpack;
        _src = rhs._src;
        _callComplete = rhs._callComplete;
        _state = rhs._state;
        return *this;
    }

    void me::_runCallComplete() {
        _state = LINKED; // set to LINKED to prevent infinite loop.
        if(_callComplete) _callComplete->run();
    }
} // namespace nm
