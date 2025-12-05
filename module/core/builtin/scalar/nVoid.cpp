#include "core/builtin/scalar/nVoid.hpp"

#include "core/worker/visitor/visitor.hpp"
#include "core/type/as/impliAses.hpp"
#include "core/bridge/cpp/tbridger.hpp"

namespace by {

    BY(DEF_ME(nVoid), DEF_VISIT())

    nbool me::wVoidType::isImmutable() const { return true; }

    const std::string& me::wVoidType::getName() const {
        static std::string inner("void");
        return inner;
    }

    const impliAses& me::wVoidType::_getImpliAses() const {
        static impliAses inner;
        return inner;
    }

    me& me::singleton() {
        static me inner;
        return inner;
    }

    tstr<scalar> me::bitwiseNot() const { return *this; }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }

    tstr<scalar> me::_add(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_sub(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_mul(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_div(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_mod(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_bitwiseAnd(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_bitwiseXor(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_bitwiseOr(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_lshift(const scalar& rhs, nbool reversed) const { return *this; }

    tstr<scalar> me::_rshift(const scalar& rhs, nbool reversed) const { return *this; }

    nbool me::_eq(const scalar& rhs) const { return false; }

    nbool me::_ne(const scalar& rhs) const { return false; }

    nbool me::_gt(const scalar& rhs) const { return false; }

    nbool me::_lt(const scalar& rhs) const { return false; }

    nbool me::_ge(const scalar& rhs) const { return false; }

    nbool me::_le(const scalar& rhs) const { return false; }

    nbool me::_logicalAnd(const scalar& rhs) const { return false; }

    nbool me::_logicalOr(const scalar& rhs) const { return false; }

    scalar& me::_mov(const scalar& rhs) { return *this; }
} // namespace by
