#pragma once

#include "core/frame/frameInteractable.hpp"
#include "core/type/funcMgdType.hpp"
#include "core/ast/baseFunc.hpp"

namespace nm {

    class getExpr;
    class func;
    class defNestedFuncExpr;

    /// closure is a proxy for a func with captured object scope.
    /// when you input arguments, its object scope was replaced to the captured scope.
    /// so eventually, you don't need to refer any object if you carry a func with closure.
    class _nout closure: public baseFunc {
        NM(ME(closure, baseFunc), CLONE(closure), VISIT())

    public:
        closure(const baseObj& org, const baseFunc& func);

    public:
        const ntype& getType() const override;

        using super::subs;
        scope& subs() override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        const modifier& getModifier() const override;

        str run(const args& a) override;

        const baseObj& getOrigin() const override;

        const baseFunc& getFunc() const;

        using super::getParams;
        params& getParams() override;

        const node& getRet() const override;

        const src& getSrc() const override;

        static me* make(const node& e);

    private:
        static me* _make(const func& e);
        static me* _make(const getExpr& e);
        static me* _make(defNestedFuncExpr& e);

    private:
        tstr<baseObj> _org;
        tstr<baseFunc> _func;
        funcMgdType _type;
    };
} // namespace nm
