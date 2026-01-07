/// @file
#pragma once

#include "core/ast/exprs/getExpr.hpp"

namespace by {

    class visitor;
    class genericOrigin;

    /// @ingroup core
    /// @brief Generic type access expression
    /// @details Uses @ref genericOrigin objects to create generic types.
    ///
    /// @section lazy_but_not_lazy Lazy but Not Lazy
    /// genericOrigin lazily creates generic types using `eval()` with given type parameters. However, since they are
    /// created in advance by the @ref verifier before program execution, they aren't actually created lazily.
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
