#include "core/builtin/primitive/nVoid.hpp"

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

    tstr<arithmeticObj> me::bitwiseNot() const { return *this; }

    const baseObj& me::getOrigin() const {
        static tbaseObjOrigin<me> org(tbridger<me>::ctor().ctor<me>().subs());
        const baseObj& supers = super::getOrigin();
        return &supers == this ? org : supers;
    }

    tstr<arithmeticObj> me::_add(const arithmeticObj& rhs, nbool reversed) const { return *this; }

    tstr<arithmeticObj> me::_sub(const arithmeticObj& rhs, nbool reversed) const { return *this; }

    tstr<arithmeticObj> me::_mul(const arithmeticObj& rhs, nbool reversed) const { return *this; }

    tstr<arithmeticObj> me::_div(const arithmeticObj& rhs, nbool reversed) const { return *this; }

    tstr<arithmeticObj> me::_mod(const arithmeticObj& rhs, nbool reversed) const { return *this; }

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

    nbool me::_eq(const arithmeticObj& rhs) const { return false; }

    nbool me::_ne(const arithmeticObj& rhs) const { return false; }

    nbool me::_gt(const arithmeticObj& rhs) const { return false; }

    nbool me::_lt(const arithmeticObj& rhs) const { return false; }

    nbool me::_ge(const arithmeticObj& rhs) const { return false; }

    nbool me::_le(const arithmeticObj& rhs) const { return false; }

    nbool me::_logicalAnd(const arithmeticObj& rhs) const { return false; }

    nbool me::_logicalOr(const arithmeticObj& rhs) const { return false; }

    arithmeticObj& me::_mov(const arithmeticObj& rhs) { return *this; }
} // namespace by
