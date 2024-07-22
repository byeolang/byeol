#include "origin.hpp"
#include "../type/mgdType.hpp"

namespace nm {

    NM(DEF_ME(origin))

    me::origin(const mgdType& newType): super(), _type(newType) {}
    me::origin(const mgdType& newType, scope& shares, scope& owns): super(shares, owns), _type(newType) {}
    me::origin(const mgdType& newType, const obj& subpack, nbool isComplete): super(isComplete), _type(newType), _subpack(subpack) {}
    me::origin(const me& rhs): super(rhs), _type(rhs._type), _subpack(rhs._subpack), _src(rhs._src) {}

    me& me::operator=(const me& rhs) {
        if(this == &rhs) return*this;

        super::operator=(rhs);
        return _assign(rhs);
    }

    const ntype& me::getType() const {
        return _type;
    }

    const obj& me::getSubPack() const { return *_subpack; }

    const src& me::getSrc() const {
        return *_src;
    }

    clonable* me::cloneDeep() const {
        // update origin:
        //  this makes an origin object. and cloned origin object's origin should itself.
        //  but don't bind _org to this. it's circular dependency.
        NM_DI("%s.cloneDeep()", *this);
        me* ret = new me(*this);
        ret->subs().link(*(scope*) getShares().cloneDeep());
        return ret;
    }

    const baseObj& me::getOrigin() const { return *this; }

    baseObj* me::make() const {
        return new obj(*this);
    }

    void me::_setType(const mgdType& new1) {
        _type = new1;
    }

    void me::_setSubPack(const obj& subpack) {
        _subpack.bind(subpack);
    }

    void me::_setSrc(const src& s) {
        _src.bind(s);
    }

    me& me::_assign(const me& rhs) {
        _type = rhs._type;
        _subpack = rhs._subpack;
        _src = rhs._src;
        return *this;
    }
}
