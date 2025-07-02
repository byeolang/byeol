#pragma once

#include "core/ast/scope.hpp"
#include "core/builtin/container/native/tnarr.hpp"

namespace nm {

    class defVarExpr;

    class _nout defBlock: public instance {
        NM(CLASS(defBlock, instance))

    public:
        defBlock();

    public:
        me& addCommon(node& stmt);
        me& addCommon(node* it) NM_SIDE_FUNC(it, addCommon(*it), *this);

        me& addScope(const std::string& name, node& stmt);
        me& addScope(const std::string& name, node* it) NM_SIDE_FUNC(it, addScope(name, *it), *this);

        me& expand(node& stmt);
        me& expand(node* it) NM_SIDE_FUNC(it, expand(*it), *this);

        const narr& getExpands() const;
        scope& getScope();
        const scope& getScope() const NM_CONST_FUNC(getScope())
        const narr& getCommon() const;

    private:
        tstr<scope> _scope;
        tstr<narr> _common;
        tstr<narr> _expands;
    };
}
