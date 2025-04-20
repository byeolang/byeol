#pragma once

#include "core/ast/params.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace nm {

    class _nout generalizer: public visitor {
        NM(CLASS(generalizer, visitor))

    public:
        /// @param origin of generic object.
        me& add(const obj& origin);
        me& add(const param& newParam);
        me& add(const params& newParams);

        using super::onVisit;
        nbool onVisit(const visitInfo& i, asExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, defVarExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, runExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseFunc& me, nbool) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseCtor& me, nbool) override;
        nbool onVisit(const visitInfo& i, params& me);
        nbool onVisit(const visitInfo& i, FBOExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me, nbool) override;

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
