#include "obj.hpp"

#include "../worker/visitor/visitor.hpp"
#include "baseFunc.hpp"
#include "node.inl"
#include "origin.hpp"
#include "modifier/dumModifier.hpp"

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

        static dumModifier defaultMod;
    }

    me::obj(const me& rhs): super(rhs) { _assign(rhs); }

    me::obj(): me(*new scope(), *new scope()) {}

    me::obj(const scope& subs): me(subs, *new scope()) {}

    me::obj(const scope& shares, scope& owns): super() {
        owns.link(shares);
        _subs.bind(owns);
    }

    me& me::_assign(const me& rhs) {
        scope* clonedOwns = scope::wrap<scope>(*(scope::super*) _cloneEach(rhs));
        clonedOwns->link(rhs.getShares());
        _subs.bind(*clonedOwns);

        _org.bind(rhs.getOrigin()); // don't '_org = rhs'. it doesn't work when rhs is origin class.

        return *this;
    }

    me& me::operator=(const me& rhs) {
        if(&rhs == this) return *this;

        super::operator=(rhs);

        return _assign(rhs);
    }

    const ntype& me::getType() const {
        if(!_org) return ttype<obj>::get();
        return _org->getType();
    }

    void me::onCloneDeep(const clonable& from) {
        // update obj:
        //  this makes an object. and cloned this object's origin should itself.
        //  but don't bind _org to this. it's circular dependency.
        me& rhs = (me&) from; // 'owns' will be deepcopied already when you clone().
                              // see @me::_assign() func.
        subs().link(*(scope*) rhs.getShares().cloneDeep());
    }

    scope& me::subs() { return *_subs; }

    tstr<nbicontainer> me::mySubs() const { return _subs->cloneChain(getShares()); }

    state me::getState() const {
        if(_org) return _org->getState();

        return LINKED;
    }

    void me::setState(state new1) {
        if(!_org) return;
        _org->setState(new1);
    }

    scope& me::getShares() { return _subs THEN(getNext()) THEN(template cast<scope>()); }

    scope::super& me::getOwns() { return _subs THEN(getContainer()); }

    node& me::getCallComplete() { return nulOf<node>(); }

    void me::_inFrame(frame& fr, const bicontainable& args) const {
        const node& subpack = getOrigin() THEN(getSubPack());
        if(!nul(subpack)) fr.add(subpack);
        super::_inFrame(fr, args);
    }

    str me::_onBeforeCtor() {
        str ret = (node*) getOrigin().clone();
        ret->run(baseObj::COMMON_NAME); // even if there is no @common, that's okay.
        return ret;
    }
} // namespace nm
