#pragma once

#include "defVarExpr.hpp"

namespace namu {
    class _nout defAssignExpr : public defVarExpr {
        NAMU(CLASS(defAssignExpr, defVarExpr, expr::exprType))

    public:
        defAssignExpr(const std::string& name, const node& rhs);
        defAssignExpr(const std::string& name, const node& rhs, const node& to);

    protected:
        str _onMakeNew() const override;
    };
}
