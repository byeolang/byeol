#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"

namespace namu {
    class _nout defPropExpr : public expr {
        NAMU(CLASS(defPropExpr, expr))
        friend class verifier;

    public:
        defPropExpr(const std::string& name, const node& rhs);
        defPropExpr(const std::string& name, const node& rhs, const node& to);

    public:
        using super::run;
        str run(const args& a) override;

        const std::string& getName() const;

        node& getRight();
        const node& getRight() const NAMU_UNCONST_FUNC(getRight())
        void setRight(const node& rhs);

        node& getTo();
        const node& getTo() const NAMU_UNCONST_FUNC(getTo())
        nbool isToFrame() const;
        void setTo(const node& to);

        str getEval() const override;

        clonable* cloneDeep() const override;

    private:
        scope& _getToScope();

    private:
        std::string _name;
        str _rhs;
        str _to;
    };
}
