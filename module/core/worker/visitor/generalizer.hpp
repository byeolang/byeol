/// @file
#pragma once

#include "core/ast/params.hpp"
#include "core/worker/visitor/visitor.hpp"

namespace by {

    /// @ingroup core
    /// @brief Visitor for generic type parameter replacement
    /// @details A class that handles generic type creation based on @ref visitor. Mainly used by @ref getGenericExpr,
    /// so refer to that class first. The most important part is the first line where inside a genericOrigin instance
    /// called SomeGeneric, an origin object called SomeGeneric<myObj> is stored with the key `myObj`. generalizer is
    /// used to create SomeGeneric<myObj> from SomeGeneric<T>, and at this time it simply finds `getExpr objects whose
    /// args is T` and replaces them with `getExpr whose args is myObj`, performing this across all nodes.
    class _nout generalizer: public visitor {
        BY(CLASS(generalizer, visitor))

    public:
        /// @param origin of generic object.
        me& add(const obj& origin);
        me& add(const param& newParam);
        me& add(const params& newParams);

        using super::onVisit;
        nbool onVisit(const visitInfo& i, asExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, defVarExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, evalExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseFunc& me, nbool) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseCtor& me, nbool) override;
        nbool onVisit(const visitInfo& i, params& me);
        nbool onVisit(const visitInfo& i, FBOExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me, nbool) override;

    private:
        str _findOrigin(const node& toReplace) const;
        str _findOrigin(const node* it) const BY_SIDE_FUNC(_findOrigin);
        str _findOriginFrom(const getExpr& expr) const;
        str _findOriginFrom(const getGenericExpr& expr) const;
        std::string _makeParamsKey() const;

    private:
        params _params;
        mutable std::string _paramsKey;
        tstr<obj> _org;
    };
} // namespace by
