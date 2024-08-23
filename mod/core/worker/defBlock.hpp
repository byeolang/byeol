#pragma once

#include "../ast/scope.hpp"
#include "../builtin/container/native/tnarr.hpp"

namespace nm {

    class defVarExpr;
    class _nout defBlock: public instance {
        NM(CLASS(defBlock, instance))

    public:
        defBlock();

    public:
        me& addCommon(node& stmt);
        me& addScope(const std::string& name, node& stmt);
        me& expand(node& stmt);
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
