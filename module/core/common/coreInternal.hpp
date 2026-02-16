#pragma once

#include "core/common/def.hpp"

namespace by {

    class node;
    class src;
    class mgdType;
    class baseObj;
    class modifier;
    class ntype;
    class args;
    class frames;
    class frame;
    class parser;
    class exprMaker;
    class func;
    class coreInternal {
        BY(ME(coreInternal))

    public:
        static void setSrc(node& me, const src& s);
        static void setType(baseObj& me, const mgdType& new1);
        static void setModifier(baseObj& me, const modifier& mod);
        static void setOrigin(baseObj& me, const baseObj& newOrg);
        static void setOrigin(func& me, const baseObj& newOrg);
        static ntype& getType(node& me);
        static str onEvalSub(node& me, node& sub, const args& a);
        static frames* getFrames();
        static frame* getNowFrame();
        static exprMaker& getMaker(parser& me);

    };
}
