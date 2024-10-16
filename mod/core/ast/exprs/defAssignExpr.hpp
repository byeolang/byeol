#pragma once

#include "defVarExpr.hpp"

namespace nm {
    class _nout defAssignExpr: public defVarExpr {
        NM(CLASS(defAssignExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defAssignExpr(const std::string& name, const node& rhs);
        defAssignExpr(const std::string& name, const node& rhs, const node& to, const src& s,
            const modifier& mod);
        defAssignExpr(const std::string& name, const node& type, const node& rhs, const node& to, const src& s,
            const modifier& mod);

    public:
        str getEval() const override;

    protected:
        tstr<baseObj> _onMakeNew() const override;

    private:
        str _type;
    };
}
