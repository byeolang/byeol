#include "obj.hpp"
#include "../visitor/visitor.hpp"
#include "../type/mgdType.hpp"
#include "baseFunc.hpp"
#include "node.inl"

namespace namu {

    NAMU(DEF_ME(obj), DEF_VISIT())

    me& me::_assign(const me& rhs) {
        scope* cloned = (scope*) rhs._owns->deepClone();
        cloned->setOwner(*this);
        _owns.bind(cloned);

        _shares.bind(*rhs._shares);
        _subs.bind(_makeNewSubs());
        _org = rhs._org;
        _pos = rhs._pos;
        _type = rhs._type;
        _src = rhs._src;
        // complete attribute is unique:
        //  all unique attributes looses when instance got cloned.
        _isComplete = true;

        return *this;
    }

    me::obj(): super(), _org(this), _type(nullptr), _isComplete(true) {
        _shares.bind(new scopes(new scope(*this)));
        _owns.bind(new scope(*this));
        _subs.bind(_makeNewSubs());
    }

    me::obj(const scopes& shares, const scope& owns):
            super(), _shares(shares), _owns(owns), _org(this), _type(nullptr), _isComplete(true) {
        _subs.bind(_makeNewSubs());
    }

    me::obj(mgdType* newType): super(), _org(this), _type(nullptr), _isComplete(true) {
        _shares.bind(new scopes(new scope(*this)));
        _owns.bind(new scope(*this));
        _subs.bind(_makeNewSubs());
        _setType(newType);
    }

    me::obj(mgdType* newType, const scopes& shares, const scope& owns):
            super(), _shares(shares), _owns(owns), _org(this), _type(nullptr), _isComplete(true) {
        _subs.bind(_makeNewSubs());
        _setType(newType);
    }

    me::obj(const me& rhs): super(rhs), _type(nullptr) {
        _assign(rhs);
    }

    me& me::operator=(const me& rhs) {
        if (&rhs == this) return *this;

        super::operator=(rhs);

        return _assign(rhs);
    }

    str me::run(const args& a) {
        tpriorities<baseFunc> p = subAll<baseFunc>(baseObj::CTOR_NAME, a);
        auto matches = p.getMatches();
        switch(matches.len()) {
            case 1: return run(baseObj::CTOR_NAME, a);
            case 0: return NAMU_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return NAMU_W("")*/
        return NAMU_E("ambigious call found: %s", "TODO:"), str();
    }

    const ntype& me::getType() const {
        if(nul(_type))
            return ttype<obj>::get();
        return *_type;
    }

    void me::_setType(const mgdType* new1) {
        _type = new1;
    }

    nbicontainer& me::subs() {
        return *_subs;
    }

    scopes& me::getShares() {
        return *_shares;
    }

    scope& me::getOwns() {
        return *_owns;
    }

    const obj& me::getOrigin() const {
        return *_org;
    }

    const point& me::getPos() const {
        return _pos;
    }

    nbool me::isComplete() const {
        return _isComplete;
    }

    void me::_setComplete(nbool isComplete) {
        _isComplete = isComplete;
    }

    void me::_setSrc(const src& newSrc) {
        _src.bind(newSrc);
    }

    void me::_setPos(const point& new1) {
        _pos = new1;
    }

    scopes* me::_makeNewSubs() {
        scopes* ret = new scopes(*_owns);
        ret->link(*_shares);

        return ret;
    }

    void me::_setOrigin(obj* newOrg) {
        _org = newOrg;
    }

    clonable* me::deepClone() const {
        NAMU_DW("obj: deepClone");

        me* ret = (me*) clone();
        if(_shares) ret->_shares.bind((scopes*) _shares->deepClone()); // chain's deepClone() clones first container only.
        ret->_subs.bind((scopes*) ret->_makeNewSubs());
        return ret;
    }
}
