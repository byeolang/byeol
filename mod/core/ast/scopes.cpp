#include "scopes.hpp"
#include "../builtin/container/native/tnchain.inl"

namespace namu {

    NAMU(DEF_ME(scopes))

    me::scopes() {}
    me::scopes(const super::super& arr): super(arr), _owner(arr.getOwner()) {}
    me::scopes(const super::super* arr): super(arr), _owner(arr->getOwner()) {}

    node& me::getOwner() { return *_owner; }
    void me::setOwner(const node& owner) { _owner.bind(owner); }
    me* me::cloneDeep() const { return (me*) super::cloneDeep(); }

    me* me::wrap(const super::super& toShallowWrap) const {
        return (me*) wrap<me>(toShallowWrap);
    }

    me* me::cloneChain(const super::super& until) const {
        return cloneChain<me>(*this, until);
    }

    me* me::cloneChain(const super& until) const {
        return cloneChain(nul(until) ? nulOf<super>() : until.getContainer());
    }

    me* me::cloneChain() const {
        return cloneChain(nulOf<super>());
    }
}
