#pragma once

#include "../../ast/baseObj.hpp"
#include "../../ast/scope.hpp"

namespace nm {

    class _nout arithmeticObj: public baseObj {
        NM(ADT(arithmeticObj, baseObj))

    public:
        using super::run;
        str run(const args& a) override;

        tstr<me> add(const me& rhs) const;
        using super::sub;
        tstr<me> sub(const me& rhs) const;
        tstr<me> mul(const me& rhs) const;
        tstr<me> div(const me& rhs) const;
        tstr<me> mod(const me& rhs) const;
        tstr<me> bitwiseAnd(const me& rhs) const;
        tstr<me> bitwiseXor(const me& rhs) const;
        tstr<me> bitwiseOr(const me& rhs) const;
        tstr<me> lshift(const me& rhs) const;
        tstr<me> rshift(const me& rhs) const;
        virtual tstr<me> bitwiseNot() const = 0;

        nbool eq(const me& rhs) const;
        nbool ne(const me& rhs) const;
        nbool gt(const me& rhs) const;
        nbool lt(const me& rhs) const;
        nbool ge(const me& rhs) const;
        nbool le(const me& rhs) const;
        nbool logicalAnd(const me& rhs) const;
        nbool logicalOr(const me& rhs) const;

        /// same to 'assign'.
        tstr<me> mov(const me& rhs);

        const src& getSrc() const override;

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
} // namespace nm
