#pragma once

#include "nStr.hpp"

namespace nm {

    class _nout nVoid : public primitiveObj<void> {

        class _nout wVoidType : public ntype {
        public:
            nbool isImmutable() const override;

        protected:
            const ases& _getImpliAses() const override;
        };
        NAMU(CLASS(nVoid, primitiveObj<void>, wVoidType), VISIT())

    public:
        nVoid();

    public:
        tstr<arithmeticObj> bitwiseNot() const override;
        const me& getOrigin() const override;

    public:
        static me& singletone();

    protected:
        void _onMakeSubs(scope& tray) const override;

        tstr<arithmeticObj> _add(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _sub(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _mul(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _div(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _mod(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _bitwiseAnd(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _bitwiseXor(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _bitwiseOr(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _lshift(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _rshift(const arithmeticObj& rhs, nbool reversed) const override;

        nbool _eq(const arithmeticObj& rhs) const override;
        nbool _ne(const arithmeticObj& rhs) const override;
        nbool _gt(const arithmeticObj& rhs) const override;
        nbool _lt(const arithmeticObj& rhs) const override;
        nbool _ge(const arithmeticObj& rhs) const override;
        nbool _le(const arithmeticObj& rhs) const override;
        nbool _logicalAnd(const arithmeticObj& rhs) const override;
        nbool _logicalOr(const arithmeticObj& rhs) const override;

        arithmeticObj& _mov(const arithmeticObj& rhs) override;
    };
}
