#pragma once

#include "../../ast/params.hpp"
#include "visitor.hpp"

namespace nm {

    class _nout generalizer: public visitor {
        NM(CLASS(generalizer, visitor))

    public:
        me& add(const param& newParam);

        using super::onVisit;
        nbool onVisit(const visitInfo& i, asExpr& me) override;
        nbool onVisit(const visitInfo& i, blockExpr& me) override;
        nbool onVisit(const visitInfo& i, defVarExpr& me) override;
        nbool onVisit(const visitInfo& i, runExpr& me) override;
        nbool onVisit(const visitInfo& i, baseFunc& me) override;
        nbool onVisit(const visitInfo& i, baseCtor& me) override;
        nbool onVisit(const visitInfo& i, params& me);
        nbool onVisit(const visitInfo& i, FBOExpr& me) override;
        nbool onVisit(const visitInfo& i, baseObj& me) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me) override;

    private:
        const node& _findOrigin(const node& toReplace) const;
        nbool _onVisitCtor(const visitInfo& i, func& me);

    private:
        params _params;
    };
}
