#pragma once

#include "visitor.hpp"
#include "../ast/params.hpp"

namespace nm {

    class _nout generalizer: public visitor {
        NM(CLASS(generalizer, visitor))

    public:
        me& add(const param& newParam);

        using super::onVisit;
        nbool onVisit(visitInfo i, asExpr& me) override;
        nbool onVisit(visitInfo i, blockExpr& me) override;
        nbool onVisit(visitInfo i, defVarExpr& me) override;
        nbool onVisit(visitInfo i, runExpr& me) override;
        nbool onVisit(visitInfo i, baseFunc& me) override;
        nbool onVisit(visitInfo i, ctor& me) override;
        nbool onVisit(visitInfo i, params& me);
        nbool onVisit(visitInfo i, FBOExpr& me) override;
        nbool onVisit(visitInfo i, baseObj& me) override;
        nbool onVisit(visitInfo i, getGenericExpr& me) override;

    private:
        const node& _findOrigin(const node& toReplace) const;

    private:
        params _params;
    };
}
