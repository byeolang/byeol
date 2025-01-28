#pragma once

#include "../container/tucontainable.hpp"
#include "primitiveObj.hpp"

namespace nm {

    class _nout nStr: public primitiveObj<std::string>, public tucontainable<nStr> {
        struct _nout nStrType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const ases& _getAses() const override;
        };
        NM(CLASS(nStr, primitiveObj, nStrType), VISIT())

        typedef typename tucontainable<nStr>::iter iter;
        typedef typename tucontainable<nStr>::iteration iteration;

    public:
        nStr() = default;
        nStr(nchar character);
        nStr(const nchar* val);
        nStr(const std::string& val);

    public:
        nchar operator[](nint n) const;

    public:
        nint len() const override;

        const baseObj& getOrigin() const override;

        tstr<arithmeticObj> bitwiseNot() const override;

        /// @param end is exclusive.
        tstr<nStr> substr(nint start, nint end);

        nbool in(nidx n) const;

        using super::get;
        nchar get(nidx n) const;

        void add(const iter& here, const iter& from, const iter& to) override;
        nbool add(const iter& at, const nStr& new1) override;
        nbool set(const iter& at, const nStr& new1) override;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;
        void rel() override;


    protected:
        iteration* _onMakeIteration(ncnt step, nbool isReverse) const override;

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
