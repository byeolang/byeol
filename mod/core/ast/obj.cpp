#include "obj.hpp"
#include "../visitor/visitor.hpp"
#include "baseFunc.hpp"
#include "node.inl"
#include "origin.hpp"

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

    me::obj(const me& rhs): super(rhs), _isComplete(true) {
        _assign(rhs);
    }

    me::obj(const origin& org): super(org), _org(org), _isComplete(true) {
        _assign(org);
    }

    me::obj(): super(), _org(*this), _isComplete(true) {}

    me::obj(scope& shares, scope& owns): super(), _isComplete(true) {
        owns.link(shares);
        _subs.bind(owns);
    }

    me::obj(nbool isComplete): super(), _isComplete(isComplete) {}

    me& me::_assign(const me& rhs) {
        scope* clonedOwns = scope::wrap<scope>(*(scope::super*) _cloneEach(rhs));
        clonedOwns->link(rhs.getShares());
        _subs.bind(*clonedOwns);

        _org.bind(rhs.getOrigin()); // don't '_org = rhs'. it doesn't work when rhs is origin class.
        // complete attribute is unique:
        //  all unique attributes looses when instance got cloned.
        _isComplete = true;

        return *this;
    }

    me& me::operator=(const me& rhs) {
        if (&rhs == this) return *this;

        super::operator=(rhs);

        return _assign(rhs);
    }

    const origin& me::getSubPack() const {
        me* c = (me*) this;
        return safeGet(c->_org, getSubPack());
    }

    const ntype& me::getType() const {
        if(!_org)
            return ttype<obj>::get();
        return _org->getType();
    }

    scope& me::subs() {
        return *_subs;
    }

    tstr<nbicontainer> me::mySubs() const {
        return _subs->cloneChain(getShares());
    }

    scope& me::getShares() {
        return safeGet(_subs, getNext(), cast<scope>());
    }

    scope::super& me::getOwns() {
        return safeGet(_subs, getContainer());
    }

    const baseObj& me::getOrigin() const {
        return *_org;
    }

    nbool me::isComplete() const {
        return _isComplete;
    }

    nbool me::isPreEvaluated() const {
        auto subs = mySubs();
        return nul(subs->get(baseObj::PRECTOR_NAME));
    }

    void me::_inFrame(frame& fr, const bicontainable& args) {
        const origin& subpack = safeGet(getOrigin(), getSubPack());
        if(!nul(subpack))
            fr.add(subpack);
        super::_inFrame(fr, args);
    }

    void me::_setComplete(nbool isComplete) {
        _isComplete = isComplete;
    }

    void me::_setOrigin(const origin& newOrg) {
        _org.bind(newOrg);
    }
}
