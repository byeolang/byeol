/// @file
#pragma once

#include "core/builtin/scalar/nStr.hpp"

namespace by {

    /** @ingroup core
     *  @brief Void primitive type in byeol language
     *  @details Represents void/null values and no-operation results.
     *  Used for functions that don't return values and empty expressions.
     */
    class _nout nVoid: public tscalar<void> {
        /** @ingroup core
         *  @brief Type information for void primitive
         *  @details Provides type metadata for nVoid.
         */
        class _nout wVoidType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const impliAses& _getImpliAses() const override;
        };
        BY(CLASS(nVoid, tscalar<void>, wVoidType), VISIT())

    public:
        tstr<scalar> bitwiseNot() const override;

        const baseObj& getOrigin() const override;

    public:
        static me& singleton();

    protected:
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
