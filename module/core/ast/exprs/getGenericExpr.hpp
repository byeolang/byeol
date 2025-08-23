/// @file
#pragma once

#include "core/ast/exprs/getExpr.hpp"

namespace by {

    class visitor;
    class genericOrigin;

    /// @ingroup core
    /// @brief Generic type access expression
    /// @details Represents access to generic types with type parameters (e.g., MyType<Int>).
    /// Handles generic type instantiation and type parameter binding.
    class _nout getGenericExpr: public getExpr {
        BY(CLASS(getGenericExpr, getExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        getGenericExpr(const std::string& genericName, const args& typeParams);
        getGenericExpr(const node& me, const std::string& genericName, const args& typeParams);

    protected:
        node* _onGet(node& me) const override;
    };
}
