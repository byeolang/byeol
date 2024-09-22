#pragma once

#include "baseFunc.hpp"
#include "../frame/frameInteractable.hpp"

namespace nm {
    class _nout closure : public baseFunc {
        NM(ME(closure, baseFunc), CLONE(closure), VISIT())

    public:
        closure(const scope& captured, const baseFunc& func);

    public:
        using super::subs;
        scope& subs() override;

        priorType prioritize(const args& a) const override;

        const ntype& getType() const override;

        const modifier& getModifier() const override;

        str run(const args& a) override;

        void inFrame(const bicontainable& args) override;

        void outFrame(const bicontainable& args) override;

        scope& getCaptured();
        const scope& getCaptured() const NM_CONST_FUNC(getCaptured());

        baseFunc& getFunc();
        const baseFunc& getFunc() const NM_CONST_FUNC(getFunc());

    private:
        tstr<scope> _capture;
        tstr<baseFunc> _func;
    };
}
