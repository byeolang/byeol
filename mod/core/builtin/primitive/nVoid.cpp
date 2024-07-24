#include "nVoid.hpp"
#include "../../visitor/visitor.hpp"
#include "bridge/cpp/tbridger.hpp"

namespace nm {

    NM(DEF_ME(nVoid), DEF_VISIT())

    nbool me::wVoidType::isImmutable() const { return true; }

    const ases& me::wVoidType::_getImpliAses() const {
        static ases inner;
        return inner;
    }

    me::nVoid() {}

    me& me::singletone() {
        static me inner;
        return inner;
    }

    namespace {
        static me org;
    }

    scope& me::_onMakeSubs() const {
        static scope inner = tbridger<me>::ctor().ctor<me, me>().subs();
        return inner;
    }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return *this;
    }

    const baseObj& me::getOrigin() const {
        return org;
    }

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_sub(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_mul(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_div(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_mod(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_bitwiseXor(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_bitwiseOr(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_lshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    tstr<arithmeticObj> me::_rshift(const arithmeticObj& rhs, nbool reversed) const {
        return *this;
    }

    nbool me::_eq(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_ne(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_gt(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_lt(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_ge(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_le(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_logicalAnd(const arithmeticObj& rhs) const {
        return false;
    }
    nbool me::_logicalOr(const arithmeticObj& rhs) const {
        return false;
    }

    arithmeticObj& me::_mov(const arithmeticObj& rhs) {
        return *this;
    }
}
