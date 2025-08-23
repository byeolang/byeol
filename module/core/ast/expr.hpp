/// @file
#pragma once

#include "core/ast/args.hpp"
#include "core/ast/src/src.hpp"

struct exprTest;

namespace by {

    /// @ingroup core
    /// @brief Base class for all expressions
    /// @details Abstract base class for all expression nodes in the AST.
    /// Provides evaluation semantics and expression-specific type handling.
    class _nout expr: public node {
    public:
        // expr can be casted to @ref node. it's proxy.
        /// @ingroup core
        /// @brief @ref ntype "Type" information for expression nodes
        /// @details Provides @ref ntype "type" metadata and conversion rules for
        /// @ref expr "expressions".
        class _nout exprType: public ntype {
            BY(ME(exprType, ntype))

        public:
            nbool isImpli(const type& to) const override;

            using super::asImpli;
            str asImpli(const node& from, const type& to) const override;
        };

        BY(ADT(expr, node, exprType))
        friend class exprMaker;
        friend struct ::exprTest;

    public:
        using super::subs;
        scope& subs() override;

        using super::isImpli;
        nbool isImpli(const type& to) const override;

        using super::prioritize;
        /// run of @ref expr class get treated like 'evaluate' in byeol.
        /// it should not take any @ref args "arguments" to run
        priorType prioritize(const args& a) const override;

        const src& getSrc() const override;

    private:
        void _setSrc(const src& newSrc) override;

    protected:
        tstr<src> _src;
        point _pos;
    };
} // namespace by
