#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"

namespace nm {
    class _nout defVarExpr : public expr {
        NAMU(ADT(defVarExpr, expr), VISIT())

    public:
        defVarExpr(const std::string& name, const node& rhs);
        defVarExpr(const std::string& name, const node& rhs, const node& to);

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

    protected:
        virtual str _onMakeNew() const = 0;

    private:
        std::string _name;
        str _rhs;
        str _to;
    };
}
