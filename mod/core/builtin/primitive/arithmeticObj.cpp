#include "arithmeticObj.hpp"

namespace namu {

    NAMU_DEF_ME(arithmeticObj)

#define _X(FUNC) \
    tstr<arithmeticObj> me::FUNC(const arithmeticObj& rhs) { \
        const ntype& deduced = getType().deduce(rhs); \
        if(nul(deduced)) return tstr<arithmeticObj>(); \
        nbool normalOrder = getType() == deduced; \
        const arithmeticObj& winner = getType() == deduced ? *this : rhs; \
        const arithmeticObj& loser = getType() == deduced ? rhs : *this; \
        return winner._ ## FUNC(loser, !normalOrder); \
    }

    NAMU_EACH(_X, add, sub, mul, div, mod)

#undef _X

#define _X(FUNC) \
    nbool me::FUNC(const me& rhs) const { \
        if(!rhs.isImpli(getType())) \
            return false; \
        return _ ## FUNC(rhs); \
    }

    NAMU_EACH(_X, eq, ne, gt, lt, ge, le)

#undef _X
}
