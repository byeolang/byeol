/// @file
#pragma once

#include "core/ast/modifier/modifier.hpp"

namespace by {
    class _nout dumModifier: public modifier {
        BY(CLASS(dumModifier, modifier))

    public:
        nbool isPublic() const override;
        void setPublic(nbool newPublic) override;

        nbool isExplicitOverride() const override;
        void setExplicitOverride(nbool isOverride) override;
    };
}
