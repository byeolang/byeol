#pragma once

#include "core/ast/expr.hpp"

namespace by {
    class _nout isExpr: public expr {
        NM(CLASS(isExpr, expr, expr::exprType), VISIT())

    public:
        isExpr(const node& me, const node& to);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const node& getMe() const;
        const node& getTo() const;
        void setTo(const node& new1);
        void setTo(const node* it) NM_SIDE_FUNC(setTo);

    private:
        str _me;
        str _to;
    };
}
