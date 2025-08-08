#pragma once

#include "core/ast/baseObj.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class _nout arithmeticObj: public baseObj {
        NM(ADT(arithmeticObj, baseObj))

    public:
        tstr<me> add(const me& rhs) const;
        tstr<me> add(const me* it) const NM_SIDE_FUNC(add);

        using super::sub;
        tstr<me> sub(const me& rhs) const;
        tstr<me> sub(const me* it) const NM_SIDE_FUNC(sub);

        tstr<me> mul(const me& rhs) const;
        tstr<me> mul(const me* it) const NM_SIDE_FUNC(mul);

        tstr<me> div(const me& rhs) const;
        tstr<me> div(const me* it) const NM_SIDE_FUNC(div);

        tstr<me> mod(const me& rhs) const;
        tstr<me> mod(const me* it) const NM_SIDE_FUNC(mod);

        tstr<me> bitwiseAnd(const me& rhs) const;
        tstr<me> bitwiseAnd(const me* it) const NM_SIDE_FUNC(bitwiseAnd);
        tstr<me> bitwiseXor(const me& rhs) const;
        tstr<me> bitwiseXor(const me* it) const NM_SIDE_FUNC(bitwiseXor);
        tstr<me> bitwiseOr(const me& rhs) const;
        tstr<me> bitwiseOr(const me* it) const NM_SIDE_FUNC(bitwiseOr);
        tstr<me> lshift(const me& rhs) const;
        tstr<me> lshift(const me* it) const NM_SIDE_FUNC(lshift);
        tstr<me> rshift(const me& rhs) const;
        tstr<me> rshift(const me* it) const NM_SIDE_FUNC(rshift);
        virtual tstr<me> bitwiseNot() const = 0;

        nbool eq(const me& rhs) const;
        nbool eq(const me* it) const NM_SIDE_FUNC(eq);
        nbool ne(const me& rhs) const;
        nbool ne(const me* it) const NM_SIDE_FUNC(ne);
        nbool gt(const me& rhs) const;
        nbool gt(const me* it) const NM_SIDE_FUNC(gt);
        nbool lt(const me& rhs) const;
        nbool lt(const me* it) const NM_SIDE_FUNC(lt);
        nbool ge(const me& rhs) const;
        nbool ge(const me* it) const NM_SIDE_FUNC(ge);
        nbool le(const me& rhs) const;
        nbool le(const me* it) const NM_SIDE_FUNC(le);
        nbool logicalAnd(const me& rhs) const;
        nbool logicalAnd(const me* it) const NM_SIDE_FUNC(logicalAnd);
        nbool logicalOr(const me& rhs) const;
        nbool logicalOr(const me* it) const NM_SIDE_FUNC(logicalOr);

        /// same to 'assign'.
        tstr<me> mov(const me& rhs);
        tstr<me> mov(const me* it) NM_SIDE_FUNC(mov);

    private:
        virtual tstr<me> _add(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _sub(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _mul(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _div(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _mod(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _bitwiseAnd(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _bitwiseXor(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _bitwiseOr(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _lshift(const me& rhs, nbool reversed) const = 0;
        virtual tstr<me> _rshift(const me& rhs, nbool reversed) const = 0;

        virtual nbool _eq(const me& rhs) const = 0;
        virtual nbool _ne(const me& rhs) const = 0;
        virtual nbool _gt(const me& rhs) const = 0;
        virtual nbool _lt(const me& rhs) const = 0;
        virtual nbool _ge(const me& rhs) const = 0;
        virtual nbool _le(const me& rhs) const = 0;
        virtual nbool _logicalAnd(const me& rhs) const = 0;
        virtual nbool _logicalOr(const me& rhs) const = 0;

        virtual me& _mov(const me& rhs) = 0;
    };
} // namespace by
