/// @file
#pragma once

#include "core/builtin/primitive/primitiveObj.hpp"

namespace by {

    /// @ingroup core
    /// @brief Boolean primitive type in byeol language
    /// @details Implements boolean operations, logical operations, and comparisons.
    /// Provides the fundamental boolean data type with full operator support.
    class _nout nBool: public primitiveObj<nbool> {
        /// @ingroup core
        /// @brief Type information for boolean primitive
        /// @details Provides type metadata and conversion rules for nBool.
        class _nout wBoolType: public ntype {
        public:
            nbool isImmutable() const override;
            const std::string& getName() const override;

        protected:
            const impliAses& _getImpliAses() const override;
            const ases& _getAses() const override;
        };

        BY(CLASS(nBool, primitiveObj, wBoolType), VISIT())

    public:
        nBool() = default;
        nBool(nbool val);

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
        tstr<arithmeticObj> _bitwiseOr(const arithmeticObj& rhs, nbool reversed) const override;
        tstr<arithmeticObj> _bitwiseXor(const arithmeticObj& rhs, nbool reversed) const override;
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
} // namespace by
