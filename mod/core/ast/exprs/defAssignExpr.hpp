#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"

namespace namu {

    class visitor;

    /// in fact, defAssign is a stmt, not a expression.
    class _nout defAssignExpr : public expr {
        NAMU(CLASS(defAssignExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        defAssignExpr(const std::string& name, const node& rhs);
        defAssignExpr(const node& to, const std::string& name, const node& rhs);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const std::string& getSubName() const;
        const node& getRight() const;
        node& getRight();
        const node& getTo() const NAMU_UNCONST_FUNC(getTo())
        nbool isOnDefBlock() const;
        void setOnDefBlock(nbool isOnDefBlock);
        node& getTo();
        void setTo(const node& new1);

        clonable* cloneDeep() const override;

    private:
        str _to;
        str _rhs;
        nbool _isOnDefBlock;
        std::string _name; // name of variable or lambda.
    };
}
