#pragma once

#include "modifier.hpp"

namespace nm {
    class _nout dumModifier: public modifier {
        NM(CLASS(dumModifier, modifier))

    public:
        nbool isPublic() const override;
        void setPublic(nbool newPublic) override;

        nbool isExplicitOverride() const override;
        void setExplicitOverride(nbool isOverride) override;
    };
}
