#pragma once

#include "defVarExpr.hpp"

namespace nm {
    class _nout defAssignExpr : public defVarExpr {
        NAMU(CLASS(defAssignExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defAssignExpr(const std::string& name, const node& rhs);
        defAssignExpr(const std::string& name, const node& rhs, const node& to);

    protected:
        str _onMakeNew() const override;
    };
}
