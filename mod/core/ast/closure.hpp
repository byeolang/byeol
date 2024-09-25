#pragma once

#include "baseFunc.hpp"
#include "../frame/frameInteractable.hpp"

namespace nm {

    /// closure is a proxy for a func with captured object scope.
    /// when you input arguments, its object scope was replaced to the captured scope.
    /// so eventually, you don't need to refer any object if you carry a func with closure.
    class _nout closure : public baseFunc {
        NM(ME(closure, baseFunc), CLONE(closure), VISIT())

    public:
        closure(const node& capture, const baseFunc& func);

    public:
        using super::subs;
        scope& subs() override;

        priorType prioritize(const args& a) const override;

        const ntype& getType() const override;

        const modifier& getModifier() const override;

        str run(const args& a) override;

        node& getCapture();
        const node& getCapture() const NM_CONST_FUNC(getCapture());

        baseFunc& getFunc();
        const baseFunc& getFunc() const NM_CONST_FUNC(getFunc());

    private:
        str _capture;
        tstr<baseFunc> _func;
    };
}
