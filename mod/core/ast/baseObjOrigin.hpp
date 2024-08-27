#pragma once

#include "baseObj.hpp"

namespace nm {

    class _nout baseObjOrigin : public baseObj {
        NM(CLASS(baseObjOrigin, baseObj))

    public:
        baseObjOrigin() = default;
        baseObjOrigin(const src& s, const scope& subs);

    public:
        const obj& getSubPack() const override;

        const src& getSrc() const override;

        const baseObj& getOrigin() const override;

        using super::subs;
        scope& subs() override;

    private:
        tstr<src> _src;
        tstr<scope> _subs;
    };
}
