#pragma once

#include "../../ast/baseFunc.hpp"

namespace nm {
    /// tApiBridge is a function template for API of primitive types.
    /// primitiveObj can't be tcppbridge object. it's already obj of namu.
    /// that's why this kind of func template required.
    template <typename T, typename Ret>
    class tApiBridge : public baseFunc {
        NAMU(ADT(tApiBridge, baseFunc))

    public:
        using super::run;
        str run(const args& a) override {
            T& cast = a.getMe().cast<T>();
            if(nul(cast)) return NAMU_W("cast is null"), str();

            return _onRun(cast, a);
        }

    protected:
        virtual str _onRun(T& cast, const args& a) const = 0;
        str getRet() const override {
            return getEval();
        }
        str getEval() const override {
            static str inner(new Ret());
            return inner;
        }
    };
}
