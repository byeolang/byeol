#include "origin.hpp"

#include "../type/mgdType.hpp"
#include "exprs/runExpr.hpp"
#include "frame/frameInteract.hpp"
#include "../builtin/err/baseErr.hpp"
#include "../frame/thread.hpp"

namespace nm {

    NM(DEF_ME(origin))

    me::origin(const mgdType& newType):
        super(), _type(newType), _src(dumSrc::singleton()), _state(RELEASED) {
        _setOrigin(*this);
    }

    me::origin(const mgdType& newType, const node& subpack):
        super(), _type(newType), _subpack(subpack), _src(dumSrc::singleton()), _state(RELEASED) {
        _setOrigin(*this);
    }

    me::origin(const me& rhs): super(rhs), _type(rhs._type) {
        _assign(rhs);
        // usually all obj called by copyctor is always complete object.
        // but, origin obj may not.
    }

    me& me::operator=(const me& rhs) {
        WHEN(this == &rhs).ret(*this);

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

    const node& me::getSubPack() const { return *_subpack; }

    const src& me::getSrc() const { return *_src; }

    void me::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;
        // update origin:
        //  this makes an origin object. and cloned origin object's origin should itself.
        //  but don't bind _org to this. it's circular dependency.
        //
        // here subs is scope for 'owns':
        //  as you may know, all obj constructs its scope just like, 'owns' -> 'shares' flow.
        //  so when you get `subs()`, it's `owns`.
        subs().link(*(scope*) rhs.getShares().cloneDeep());
        if(rhs._mod) _mod.bind((const modifier*) rhs._mod->cloneDeep());
        if(rhs._callComplete) _callComplete.bind((const node*) rhs._callComplete->cloneDeep());
    }

    baseObj* me::make() const { return new obj(*this); }

    node& me::getCallComplete() { return *_callComplete; }

    void me::setCallComplete(const node& new1) { _callComplete.bind(new1); }

    nbool me::isComplete() const { return _callComplete.isBind(); }

    clonable* me::clone() const { return new obj(*this); }

    clonable* me::cloneDeep() const {
        me* ret = new me(*this);
        ret->onCloneDeep(*this);
        return ret;
    }

    void me::_setType(const mgdType& new1) { _type = new1; }

    void me::_setSubPack(const node& subpack) { _subpack.bind(subpack); }

    void me::_setSrc(const src& s) { _src.bind(s); }

    void me::_setModifier(const modifier& mod) { _mod.bind(mod); }

    me& me::_assign(const me& rhs) {
        _type = rhs._type;
        _subpack = rhs._subpack;
        _src = rhs._src;
        _callComplete = rhs._callComplete;
        _state = rhs._state;
        _setOrigin(*this); // in super(rhs), it assign _org with rhs._org, not 'this' ptr.
        return *this;
    }

    void me::_runCallComplete() {
        _state = LINKED; // set to LINKED to prevent infinite loop.
        if(_callComplete) _callComplete->run();
    }
} // namespace nm
