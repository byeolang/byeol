#pragma once

#include "baseObj.hpp"
#include "../frame/frameInteractable.hpp"

namespace nm {

    class getExpr;
    class func;
    class defNestedFuncExpr;

    /// closure is a proxy for a func with captured object scope.
    /// when you input arguments, its object scope was replaced to the captured scope.
    /// so eventually, you don't need to refer any object if you carry a func with closure.
    class _nout closure: public baseObj {
        class _nout closureType : public ntype {
            NM(ME(closureType, ntype))

        protected:
            const ases& _getImpliAses() const override;
        };

        NM(CLASS(closure, baseObj), VISIT())

    public:
        closure(const baseObj& org, const baseFunc& func);

    public:
        using super::subs;
        scope& subs() override;

        priorType prioritize(const args& a) const override;

        const modifier& getModifier() const override;

        str run(const args& a) override;

        const baseObj& getOrigin() const override;

        const baseFunc& getFunc() const;

        params& getParams();
        const params& getParams() const NM_CONST_FUNC(getParams())

        const src& getSrc() const override;

        static me* make(const node& e);

    private:
        static me* _make(const func& e);
        static me* _make(const getExpr& e);
        static me* _make(defNestedFuncExpr& e);

    private:
        tstr<baseObj> _org;
        tstr<baseFunc> _func;
    };
} // namespace nm
