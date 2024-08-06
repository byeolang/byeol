#pragma once

#include "../func.hpp"

namespace nm {
    class _nout ctor : public func {
        NM(CLASS(ctor, func))

    public:
        ctor(const params& ps);

    public:
        str run(const args& a) override;
        str getEval() const override;
        const obj& getOrigin() const;
    };
}
