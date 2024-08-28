#pragma once

#include "defVarExpr.hpp"

namespace nm {
    class _nout defAssignExpr: public defVarExpr {
        NM(CLASS(defAssignExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defAssignExpr(const std::string& name, const node& rhs);
        defAssignExpr(const std::string& name, const node& rhs, const node& to, const src& s,
            const modifier& mod);

    protected:
        tstr<baseObj> _onMakeNew() const override;
    };
}
