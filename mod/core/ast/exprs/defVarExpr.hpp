#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"
#include "../modifier/modifier.hpp"

namespace nm {

    class assignExpr;
    class _nout defVarExpr: public expr {
        NM(ADT(defVarExpr, expr), VISIT())

    public:
        defVarExpr(const std::string& name, const node& rhs);
        defVarExpr(const std::string& name, const node& rhs, const node& to, const src& src, const modifier& mod);

    public:
        using super::run;
        str run(const args& a) override;

        const std::string& getName() const;

        node& getRight();
        const node& getRight() const NM_CONST_FUNC(getRight())
        void setRight(const node& rhs);

        node& getTo();
        const node& getTo() const NM_CONST_FUNC(getTo())
        nbool isToFrame() const;
        void setTo(const node& to);

        str getEval() const override;

        clonable* cloneDeep() const override;

        str makeNew() const;

    protected:
        virtual tstr<baseObj> _onMakeNew() const = 0;

    private:
        std::string _name;
        str _rhs;
        str _to;
        tstr<src> _src;
        tstr<modifier> _mod;
    };
}
