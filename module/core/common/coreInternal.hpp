#pragma once

#include "core/common/dep.hpp"

namespace by {

    class node;
    class src;
    class mgdType;
    class baseObj;
    class modifier;
    class coreInternal {
        BY(ME(coreInternal))

    public:
        static void setSrc(node& me, const src& s);
        static void setType(baseObj& me, const mgdType& new1);
        static void setModifier(baseObj& me, const modifier& mod);
        static void setOrigin(baseObj& me, const baseObj& newOrg);
    };
}
