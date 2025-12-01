#include "core/ast/obj.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/node.inl"
#include "core/ast/origin.hpp"
#include "core/ast/modifier/dumModifier.hpp"

namespace by {

    BY(DEF_ME(obj), DEF_VISIT())

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
        scope* clonedOwns = scope::wrap(*(scope::super*) _cloneEach(rhs));
        clonedOwns->link(rhs.getShares());
        _subs.bind(clonedOwns);
        _org.bind(rhs.getOrigin()); // don't '_org = rhs'. it doesn't work when rhs is origin class.

        return *this;
    }

    me& me::operator=(const me& rhs) {
        WHEN(&rhs == this) .ret(*this);

        super::operator=(rhs);

        return _assign(rhs);
    }

    const ntype& me::getType() const {
        WHEN(!_org) .ret(ttype<obj>::get());
        return _org->getType();
    }

    void me::onCloneDeep(const clonable& from) {
        // update obj:
        //  this makes an object. and cloned this object's origin should itself.
        //  but don't bind _org to this. it's circular dependency.
        me& rhs = (me&) from; // 'owns' will be deepcopied already when you clone().
                              // see @me::_assign() func.
        subs().link((scope&) *(rhs.getShares() TO(cloneDeep())));
    }

    scope& me::subs() { return _subs ? *_subs : dumScope::singleton(); }

    tstr<nbicontainer> me::mySubs() const { return _subs->cloneChain(&getShares()); }

    state me::getState() const {
        WHEN(_org) .ret(_org->getState());

        return LINKED;
    }

    void me::setState(state new1) {
        WHEN(!_org) .ret();
        _org->setState(new1);
    }

    scope& me::getShares() { return _subs TO(getNext()) TO(template cast<scope>()) OR.ret(dumScope::singleton()); }

    scope::super& me::getOwns() { return _subs ? _subs->getContainer() : dumScope::singleton(); }

    node* me::getCallComplete() { return nullptr; }

    void me::_inFrame(frame& fr, const bicontainable* args) const {
        const node& subpack = getOrigin().getSubPack();
        fr.add(subpack);
        super::_inFrame(fr, args);
    }

    str me::_onBeforeCtor() {
        str ret = (node*) getOrigin().clone();
        ret->eval(baseObj::COMMON_NAME); // even if there is no @common, that's okay.
        return ret;
    }
} // namespace by
