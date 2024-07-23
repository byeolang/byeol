#include "arithmeticObj.hpp"
#include "../../ast/node.inl"
#include "nVoid.hpp"

namespace nm {

    NM_DEF_ME(arithmeticObj)

#define _X(FUNC) \
    tstr<arithmeticObj> me::FUNC(const arithmeticObj& rhs) const { \
        const ntype& deduced = getType().deduce(rhs); \
        if(deduced.isSub<nVoid>()) return tstr<arithmeticObj>(nVoid::singletone()); \
        nbool normalOrder = getType() == deduced; \
        const arithmeticObj& winner = getType() == deduced ? *this : rhs; \
        const arithmeticObj& loser = getType() == deduced ? rhs : *this; \
        return winner._ ## FUNC(loser, !normalOrder); \
    }

    NM_EACH(_X, add, sub, mul, div, mod, bitwiseAnd, bitwiseXor, bitwiseOr, rshift, lshift)

#undef _X

#define _X(FUNC) \
    nbool me::FUNC(const me& rhs) const { \
        if(!rhs.isImpli(getType())) \
            return false; \
        return _ ## FUNC(rhs); \
    }

    NM_EACH(_X, eq, ne, gt, lt, ge, le, logicalAnd, logicalOr)

#undef _X

    str me::run(const args& a) {
        baseFunc& found = sub<baseFunc>(baseObj::CTOR_NAME, a);
        if(nul(found)) return NM_W("there is no such ctor."), str();

        return run(baseObj::CTOR_NAME, a);
    }

    tstr<me> me::mov(const me& rhs) {
        if(!rhs.isImpli(getType()))
            return tstr<me>();

        return tstr<me>(_mov(rhs));
    }

    const src& me::getSrc() const {
        return dumSrc::singletone();
    }
}
