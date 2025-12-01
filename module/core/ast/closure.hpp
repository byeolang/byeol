/// @file
#pragma once

#include "core/frame/frameInteractable.hpp"
#include "core/type/funcType.hpp"
#include "core/ast/baseFunc.hpp"

namespace by {

    class getExpr;
    class func;
    class defNestedFuncExpr;

    /// @ingroup core
    /// @brief @ref func closure with captured @ref obj "object" scope
    /// @details Proxy for @func with captured @ref obj "object" scope for closures.
    /// When @ref args "arguments" are provided, @ref obj "object" scope is replaced with captured scope.
    /// @note Allows carrying functions without needing to reference any object.
    class _nout closure: public baseFunc {
        BY(ME(closure, baseFunc), CLONE(closure), VISIT())

    public:
        closure(const baseObj& org, const baseFunc& func);

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        const modifier& getModifier() const override;

        using super::eval;
        str eval(const args& a) override;

        const baseObj& getOrigin() const override;

        const baseFunc& getFunc() const;

        using super::getParams;
        params& getParams() override;

        const node* getRet() const override;

        const src& getSrc() const override;

        static me* make(const node& e);
        static me* make(const node* it) BY_SIDE_FUNC(make);

    private:
        static me* _make(const func& e);
        static me* _make(const func* it) BY_SIDE_FUNC(_make) static me* _make(const getExpr& e);
        static me* _make(const getExpr* it) BY_SIDE_FUNC(_make);
        static me* _make(defNestedFuncExpr& e);
        static me* _make(defNestedFuncExpr* it) BY_SIDE_FUNC(_make);

    private:
        tstr<baseObj> _org;
        tstr<baseFunc> _func;
        funcType _type;
    };
} // namespace by
