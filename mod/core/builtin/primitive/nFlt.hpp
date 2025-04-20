#pragma once

#include "core/builtin/primitive/nStr.hpp"

namespace nm {

    class _nout nFlt: public primitiveObj<nflt> {
        class _nout wFltType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const impliAses& _getImpliAses() const override;
            const ases& _getAses() const override;
        };

        NM(CLASS(nFlt, primitiveObj, wFltType), VISIT())

    public:
        nFlt() = default;
        nFlt(nflt val);

    public:
        tstr<arithmeticObj> bitwiseNot() const override;

        const baseObj& getOrigin() const override;

    protected:
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
} // namespace nm
