#include "core/builtin/primitive/arithmeticObj.hpp"

#include "core/ast/node.inl"
#include "core/builtin/primitive/nVoid.hpp"

namespace by {

    NM_DEF_ME(arithmeticObj)

#define _X(FUNC)                                                                   \
    tstr<arithmeticObj> me::FUNC(const arithmeticObj& rhs) const {                 \
        const auto& dummy = nVoid::singleton(); \
        const ntype& deduced = getType().deduce(rhs) OR.exErr(TYPE_NOT_DEDUCED).ret(dummy);                   \
        WHEN(deduced.isSub<nVoid>()).ret(dummy); \
                                                \
        nbool normalOrder = getType() == deduced;                                  \
        const arithmeticObj& winner = getType() == deduced ? *this : rhs;          \
        const arithmeticObj& loser = getType() == deduced ? rhs : *this;           \
        return winner._##FUNC(loser, !normalOrder);                                \
    }

    NM_EACH(_X, add, sub, mul, div, mod, bitwiseAnd, bitwiseXor, bitwiseOr, rshift, lshift)

#undef _X

#define _X(FUNC)                                  \
    nbool me::FUNC(const me& rhs) const {         \
        WHEN(!rhs.isImpli(getType())).ret(false); \
        return _##FUNC(rhs);                      \
    }

        NM_EACH(_X, eq, ne, gt, lt, ge, le, logicalAnd, logicalOr)

#undef _X

            tstr<me> me::mov(const me& rhs) {
                WHEN(!rhs.isImpli(getType())).ret(tstr<me>());

                return tstr<me>(_mov(rhs));
            }
} // namespace by
