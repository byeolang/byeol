#include "obj.hpp"
#include "../visitor/visitor.hpp"
#include "../type/mgdType.hpp"
#include "baseFunc.hpp"
#include "node.inl"

namespace nm {

    NM(DEF_ME(obj), DEF_VISIT())

    namespace {
        scope::defaultContainer* _cloneEach(const me& rhs) {
            auto* new1 = new scope::defaultContainer();
            rhs.getOwns().each([&](const auto& key, const node& val) {
                new1->add(key, (node*) val.clone());
                return true;
            });
            return new1;
        }
    }

    me& me::_assign(const me& rhs) {
        scope* clonedOwns = scope::wrap<scope>(*(scope::super*) _cloneEach(rhs));
        clonedOwns->link(rhs.getShares());
        _subs.bind(*clonedOwns);

        _subpack = rhs._subpack;
        _org = rhs._org;
        _type = rhs._type;
        _src = rhs._src;
        // complete attribute is unique:
        //  all unique attributes looses when instance got cloned.
        _isComplete = true;
        return *this;
    }

    me::obj(): super(), _org(this), _type(nullptr), _isComplete(true) {
        _subs.bind(new scope(*new scope::defaultContainer(), *new scope()));
    }

    me::obj(scope& shares, scope& owns): super(), _org(this), _type(nullptr), _isComplete(true) {
        owns.link(shares);
        _subs.bind(owns);
    }

    me::obj(mgdType* newType): super(), _org(this), _type(nullptr), _isComplete(true) {
        _subs.bind(new scope(*new scope::defaultContainer(), *new scope()));
        _setType(newType);
    }

    me::obj(mgdType* newType, scope& shares, scope& owns): super(), _org(this), _type(nullptr),
        _isComplete(true) {
        owns.link(shares);
        _subs.bind(owns);
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
            case 0: return NM_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return NM_W("")*/
        return NM_E("ambigious call found: %s", "TODO:"), str();
    }

    const ntype& me::getType() const {
        if(nul(_type))
            return ttype<obj>::get();
        return *_type;
    }

    void me::_setType(const mgdType* new1) {
        _type = new1;
    }

    scope& me::subs() {
        return *_subs;
    }

    tstr<nbicontainer> me::mySubs() const {
        return _subs->cloneChain(getShares());
    }

    scope& me::getShares() {
        if(!_subs) return nulOf<scope>();
        return _subs->getNext().cast<scope>();
    }

    scope::super& me::getOwns() {
        if(!_subs) return nulOf<scope::super>();
        return _subs->getContainer();
    }

    const obj& me::getOrigin() const {
        return *_org;
    }

    const obj& me::getSubPack() const {
        return *_subpack;
    }

    nbool me::isComplete() const {
        return _isComplete;
    }

    nbool me::isPreEvaluated() const {
        auto subs = mySubs();
        return nul(subs->get(baseObj::PRECTOR_NAME));
    }

    void me::_inFrame(frame& fr, const bicontainable& args) {
        if(_subpack)
            fr.add(*_subpack);
        super::_inFrame(fr, args);
    }

    void me::_setComplete(nbool isComplete) {
        _isComplete = isComplete;
    }

    void me::_setOrigin(obj* newOrg) {
        _org = newOrg;
    }

    void me::_setSubPack(const obj& subpack) {
        _subpack.bind(subpack);
    }

    clonable* me::cloneDeep() const {
        NM_DI("%s.cloneDeep()", *this);
        me* ret = new me(*this);
        ret->subs().link(*(scope*) getShares().cloneDeep());
        return ret;
    }
}
