#pragma once

#include "../expr.hpp"

namespace nm {

    class visitor;

    class _nout runExpr : public expr {
        NM(CLASS(runExpr, expr, expr::exprType), VISIT())
        friend class parser;

    public:
        runExpr(const node& meObj, const node& subject, const args& a);
        runExpr(const node& meObj, const args& a);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;

        node& getMe();
        const node& getMe() const NM_CONST_FUNC(getMe())
        void setMe(const node& newMe);

        node& getSubj();
        const node& getSubj() const;
        void setSubj(const node& new1);

        args& getArgs();
        const args& getArgs() const;

        clonable* cloneDeep() const override;

    private:
        str _getSub(str me, const args& a) const;

    private:
        str _me;
        args _args;
        mutable str _subject;
    };
}
