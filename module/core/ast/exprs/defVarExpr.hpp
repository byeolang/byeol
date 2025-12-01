/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/modifier/modifier.hpp"

namespace by {

    class assignExpr;

    /// @ingroup core
    /// @brief Variable definition expression base class
    /// @details Abstract base class for variable definition expressions.
    /// Handles variable declaration with type, initial value, and modifiers.
    class _nout defVarExpr: public expr {
        BY(ADT(defVarExpr, expr), VISIT())

    public:
        defVarExpr(const std::string& name, const node* rhs);
        defVarExpr(const std::string& name, const node* rhs, const node* to, const src& src, const modifier& mod);

    public:
        using super::eval;
        str eval(const args& a) override;

        const std::string& getName() const;

        node* getRight();
        const node* getRight() const BY_CONST_FUNC(getRight())
        void setRight(const node& rhs);

        node* getTo();
        const node* getTo() const BY_CONST_FUNC(getTo())
        nbool isToFrame() const;
        void setTo(const node& to);

        str infer() const override;

        void onCloneDeep(const clonable& rhs) override;

        str makeNewOrigin();

        const modifier& getNewModifier() const;

    protected:
        virtual str _onMakeNew() = 0;

    private:
        str _makeNew();

    private:
        std::string _name;
        str _rhs;
        str _to;
        tstr<src> _src;
        tstr<modifier> _mod;
    };
} // namespace by
