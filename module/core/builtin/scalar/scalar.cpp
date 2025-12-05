#include "core/builtin/scalar/scalar.hpp"

#include "core/ast/node.inl"
#include "core/builtin/scalar/nVoid.hpp"

namespace by {

    BY_DEF_ME(scalar)

#define _X(FUNC)                                                                            \
    tstr<scalar> me::FUNC(const scalar& rhs) const {                          \
        const auto& dummy = nVoid::singleton();                                             \
        const ntype& promoted = getType().promote(rhs) OR.exErr(TYPE_NOT_DEDUCED).ret(dummy); \
        WHEN(promoted.isSub<nVoid>()) .ret(dummy);                                           \
                                                                                            \
        nbool normalOrder = getType() == promoted;                                           \
        const scalar& winner = getType() == promoted ? *this : rhs;                   \
        const scalar& loser = getType() == promoted ? rhs : *this;                    \
        return winner._##FUNC(loser, !normalOrder);                                         \
    }

    BY_EACH(_X, add, sub, mul, div, mod, bitwiseAnd, bitwiseXor, bitwiseOr, rshift, lshift)

#undef _X

#define _X(FUNC)                                   \
    nbool me::FUNC(const me& rhs) const {          \
        WHEN(!rhs.isImpli(getType())) .ret(false); \
        return _##FUNC(rhs);                       \
    }

        BY_EACH(_X, eq, ne, gt, lt, ge, le, logicalAnd, logicalOr)

#undef _X

            tstr<me> me::mov(const me& rhs) {
                WHEN(!rhs.isImpli(getType())) .ret(tstr<me>());

                return tstr<me>(_mov(rhs));
            }
} // namespace by
