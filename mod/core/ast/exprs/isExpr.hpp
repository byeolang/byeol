#pragma once

#include "../expr.hpp"

namespace nm {
    class _nout isExpr : public expr {
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

    private:
        str _me;
        str _to;
    };
}
