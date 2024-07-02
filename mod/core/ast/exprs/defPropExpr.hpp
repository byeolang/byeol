#pragma once

#include "defVarExpr.hpp"

namespace nm {
    class _nout defPropExpr : public defVarExpr {
        NAMU(CLASS(defPropExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defPropExpr(const std::string& name, const node& rhs);
        defPropExpr(const std::string& name, const node& rhs, const node& to);

    protected:
        str _onMakeNew() const override;
    };
}
