#pragma once

#include "baseObj.hpp"

namespace nm {

    class _nout baseObjOrigin : public baseObj {
        NM(CLASS(baseObjOrigin, baseObj))

    public:
        baseObjOrigin() = default;
        baseObjOrigin(const src& s, const scope& subs);
        baseObjOrigin(const src& s, const scope& subs, const modifier& mod);

    public:
        const obj& getSubPack() const override;

        const src& getSrc() const override;

        const baseObj& getOrigin() const override;

        using super::subs;
        scope& subs() override;

        const modifier& getModifier() const override;

    private:
        tstr<src> _src;
        tstr<scope> _subs;
        tstr<modifier> _mod;
    };
}
