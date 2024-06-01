#include "scope.hpp"
#include "../builtin/container/native/tnmap.inl"

namespace namu {
    
    NAMU(DEF_ME(scope))

    me::scope() {}
    me::scope(const node& owner): _owner(owner) {}

    node& me::getOwner() { return *_owner; }
    void me::setOwner(const node& owner) { _owner.bind(owner); }
    me* me::cloneDeep() const { return (me*) super::cloneDeep(); }
}
