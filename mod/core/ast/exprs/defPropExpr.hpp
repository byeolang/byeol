#pragma once

#include "defVarExpr.hpp"

namespace nm {
    class _nout defPropExpr: public defVarExpr {
        NM(CLASS(defPropExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defPropExpr(const std::string& name, const node& rhs);
        defPropExpr(const std::string& name, const node& rhs, const node& to, const src& s, const modifier& mod);

    protected:
        tstr<baseObj> _onMakeNew() const override;
    };
}
