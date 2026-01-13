/// @file
#pragma once

#include "core/ast/scope.hpp"
#include "core/builtin/container/native/tnarr.hpp"

namespace by {

    class defVarExpr;

    /** @ingroup core
     *  @brief Definition block for organizing object definitions
     *  @details Manages scoped definitions, common statements, and expansion operations.
     *  Used during parsing to organize class and object member definitions.
     */
    class _nout defBlock: public instance {
        BY(CLASS(defBlock, instance))

    public:
        defBlock();

    public:
        me& addCommon(node& stmt);
        me& addCommon(node* it) BY_SIDE_FUNC(it, addCommon(*it), *this);

        me& addScope(const std::string& name, node& stmt);
        me& addScope(const std::string* name, node& stmt) BY_SIDE_FUNC(name, addScope(*name, stmt), *this);
        me& addScope(const std::string& name, node* it) BY_SIDE_FUNC(it, addScope(name, *it), *this);
        me& addScope(const std::string* name, node* stmt) BY_SIDE_FUNC(name&& stmt, addScope(*name, *stmt), *this);

        me& expand(node& stmt);
        me& expand(node* it) BY_SIDE_FUNC(it, expand(*it), *this);

        const narr& getExpands() const;
        scope& getScope();
        const scope& getScope() const BY_CONST_FUNC(getScope())
        const narr& getCommon() const;

    private:
        tstr<scope> _scope;
        tstr<narr> _common;
        tstr<narr> _expands;
    };
}
