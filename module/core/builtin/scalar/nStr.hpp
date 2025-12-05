/// @file
#pragma once

#include "core/builtin/container/tucontainable.hpp"
#include "core/builtin/scalar/tscalar.hpp"

namespace by {

    /// @ingroup core
    /// @brief String primitive type in byeol language
    /// @details Implements string operations, character access, and container functionality.
    /// Provides the fundamental string data type with arithmetic and container operations.
    class _nout nStr: public tscalar<std::string>, public tucontainable<nStr> {
        struct _nout nStrType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const ases& _getAses() const override;
        };
        BY(CLASS(nStr, tscalar, nStrType), VISIT())

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

        tstr<scalar> bitwiseNot() const override;

        /// @param end is exclusive.
        tstr<nStr> substr(nint start, nint end);

        nbool in(nidx n) const;

        using super::get;
        nchar get(nidx n) const;

        using tucontainable<nStr>::add;
        void add(const iter& here, const iter& from, const iter& to) override;
        nbool add(const iter& at, const nStr& new1) override;

        using tucontainable<nStr>::set;
        nbool set(const iter& at, const nStr& new1) override;

        using tucontainable<nStr>::del;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;

        void rel() override;


    protected:
        iteration* _onMakeIteration(ncnt step, nbool isReversed) const override;

        tstr<scalar> _add(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _sub(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _mul(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _div(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _mod(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseAnd(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseXor(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseOr(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _lshift(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _rshift(const scalar& rhs, nbool reversed) const override;

        nbool _eq(const scalar& rhs) const override;
        nbool _ne(const scalar& rhs) const override;
        nbool _gt(const scalar& rhs) const override;
        nbool _lt(const scalar& rhs) const override;
        nbool _ge(const scalar& rhs) const override;
        nbool _le(const scalar& rhs) const override;
        nbool _logicalAnd(const scalar& rhs) const override;
        nbool _logicalOr(const scalar& rhs) const override;

        scalar& _mov(const scalar& rhs) override;
    };
} // namespace by
