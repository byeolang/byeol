/// @file
#pragma once

#include "core/builtin/scalar/tscalar.hpp"

namespace by {

    /** @ingroup core
     *  @brief Byte primitive type in byeol language
     *  @details Implements byte arithmetic, bitwise operations, and comparisons.
     *  Provides 8-bit unsigned integer data type with full operator support.
     */
    class _nout nByte: public tscalar<nuchar> {
        /** @ingroup core
         *  @brief Type information for byte primitive
         *  @details Provides type metadata and conversion rules for nByte.
         */
        class _nout nByteType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const impliAses& _getImpliAses() const override;
            const ases& _getAses() const override;
        };

        BY(CLASS(nByte, tscalar, nByteType), VISIT())

    public:
        nByte() = default;
        nByte(nuchar val);

    public:
        tstr<scalar> bitwiseNot() const override;

        const baseObj& getOrigin() const override;

    protected:
        tstr<scalar> _add(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _sub(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _mul(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _div(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _mod(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseAnd(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseOr(const scalar& rhs, nbool reversed) const override;
        tstr<scalar> _bitwiseXor(const scalar& rhs, nbool reversed) const override;
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
