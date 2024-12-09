#pragma once

#include "../../ast/params.hpp"
#include "visitor.hpp"

namespace nm {

    class _nout generalizer: public visitor {
        NM(CLASS(generalizer, visitor))

    public:
        /// @param origin of generic object.
        me& add(const obj& origin);
        me& add(const param& newParam);
        me& add(const params& newParams);

        using super::onVisit;
        nbool onVisit(const visitInfo& i, asExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, defVarExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, runExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, baseFunc& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, baseCtor& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, params& me);
        nbool onVisit(const visitInfo& i, FBOExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me, nbool alreadyVisited) override;

    private:
        str _findOrigin(const node& toReplace) const;
        str _findOriginFrom(const getExpr& expr) const;
        str _findOriginFrom(const getGenericExpr& expr) const;
        std::string _makeParamsKey() const;

    private:
        params _params;
        mutable std::string _paramsKey;
        tstr<obj> _org;
    };
} // namespace nm
