/// @file
#pragma once

#include "core/ast/exprs/defVarExpr.hpp"

namespace by {
    /// @ingroup core
    /// @brief Property definition expression
    /// @details Represents property declarations within objects and classes.
    /// Handles property creation with type inference and initialization.
    class _nout defPropExpr: public defVarExpr {
        BY(CLASS(defPropExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defPropExpr(const std::string& name, const node& rhs);
        defPropExpr(const std::string& name, const node& rhs, const node* to, const src& s, const modifier& mod);

    protected:
        str _onMakeNew() override;
    };
}
