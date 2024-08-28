#pragma once

#include "mockNode.hpp"
#include "baseObj.hpp"

namespace nm {

    template <typename T> class tbaseObjOrigin: public T {
        NM(CLASS(tbaseObjOrigin, T))

    public:
        tbaseObjOrigin() = default;

        tbaseObjOrigin(const src& s, const scope& subs): me(s, subs, *new modifier(true, false)) {}

        tbaseObjOrigin(const src& s, const scope& subs, const modifier& mod):
            super(), _src(s), _subs(subs), _mod(mod) {}

    public:
        const node& getSubPack() const override {
            static mockNode inner;
            return inner;
        }

        const src& getSrc() const override { return *_src; }

        const baseObj& getOrigin() const override { return *this; }

        using super::subs;

        scope& subs() override { return *_subs; }

        const modifier& getModifier() const override { return *_mod; }

    private:
        tstr<src> _src;
        tstr<scope> _subs;
        tstr<modifier> _mod;
    };
} // namespace nm
