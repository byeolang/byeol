#pragma once

#include "core/ast/exprs/defVarExpr.hpp"

namespace by {
    class _nout defAssignExpr: public defVarExpr {
        BY(CLASS(defAssignExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defAssignExpr(const std::string& name, const node* rhs);
        defAssignExpr(const std::string& name, const node* rhs, const node* to, const src& s, const modifier& mod);
        defAssignExpr(const std::string& name, const node* type, const node* rhs, const node* to, const src& s,
            const modifier& mod);

    public:
        str getEval() const override;
        const node* getExplicitType() const;
        void setExplicitType(const node& newType);

    protected:
        str _onMakeNew() override;

    private:
        str _type;
    };
}
