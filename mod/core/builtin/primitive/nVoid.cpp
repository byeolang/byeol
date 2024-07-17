#include "nVoid.hpp"
#include "../../ast/defaultCtor.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "../../visitor/visitor.hpp"

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

    void me::_onMakeSubs(scope& tray) const {
        static nVoid inner;
        tray.add(baseObj::CTOR_NAME, new defaultCtor(inner));
        tray.add(baseObj::CTOR_NAME, new defaultCopyCtor(inner));
    }

    tstr<arithmeticObj> me::bitwiseNot() const {
        return *this;
    }

    const baseObj& me::getOrigin() const {
        return nulOf<obj>();
        // TODO:
        //static nVoid inner;
        //return inner;
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
