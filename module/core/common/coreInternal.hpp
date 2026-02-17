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
    class baseCtor;
    class origin;
    class genericOrigin;
    class obj;
    typedef std::map<std::string, tstr<obj>> orgCache;
    class forExpr;
    class getExpr;
    template <typename T, typename TACTIC> class tnarr;
    typedef tnarr<node, strTactic> narr;
    template <typename T> class tbridge;
    class defArrayExpr;

    class coreInternal {
        BY(ME(coreInternal))

    public:
        static void setSrc(node& me, const src& s);
        static void setType(baseObj& me, const mgdType& new1);
        static void setModifier(baseObj& me, const modifier& mod);
        static void setOrigin(baseObj& me, const baseObj& newOrg);
        static void setOrigin(func& me, const baseObj& newOrg);
        static void setOrigin(baseCtor& me, const baseObj& newOrg);
        static void setSubPack(origin& me, const node& subpack);
        static ntype& getType(node& me);
        static str onEvalSub(node& me, node& sub, const args& a);
        static frames* getFrames();
        static frame* getNowFrame();
        static exprMaker& getMaker(parser& me);
        template <typename T>
        static tbridge<T>* makeBridge(T* real) { return new tbridge<T>(real); }
        template <typename T>
        static tbridge<T>* makeBridge() { return new tbridge<T>(); }
        template <typename T>
        static T* getReal(tbridge<T>& me) { return me._real; }
        static orgCache& getCache(genericOrigin& me);
        static str getContainer(forExpr& me);
        static node* getMe(getExpr& me);
        static args* getArgs(getExpr& me);
        static node* onGet(const getExpr& me, node& it);
        static narr& getElems(defArrayExpr& me);
    };
}
