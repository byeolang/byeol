#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"

namespace namu {

    class visitor;

    class _nout asExpr : public expr {
        NAMU(CLASS(asExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        asExpr(const node& me, const node& as);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getMe() const;
        const node& getAs() const;
        void setAs(const node& new1);
        clonable* cloneDeep() const override;

    private:
        str _me;
        str _as;
    };
}
