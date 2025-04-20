#pragma once

#include "core/common.hpp"
#include "core/type/ntype.hpp"

namespace nm {
    class _nout modifier: public instance {
        NM(CLASS(modifier, instance))

    public:
        modifier();
        modifier(nbool newPublic, nbool newExplicitOverride);

    public:
        virtual nbool isPublic() const;
        virtual void setPublic(nbool newPublic);
        virtual nbool isExplicitOverride() const;
        virtual void setExplicitOverride(nbool isOverride);

    private:
        nbool _isPublic;
        nbool _isExplicitOverride;
    };
}
