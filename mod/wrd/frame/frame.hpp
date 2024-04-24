#pragma once

#include "../ast/baseObj.hpp"
#include "scopeStack.hpp"
#include "../builtin/res/tpair.hpp"

struct immutableTest;
struct frameTest;

namespace wrd {

    class obj;
    class func;
    class _wout frame : public node { // TODO: may be obj, not node.
        WRD(CLASS(frame, node))
        friend struct ::immutableTest;
        friend struct ::frameTest;

    public:
        ~frame() override;

        /*/// @return returns tpair<node, itsOwner>. the owner has the container holding the node
        ///         which can run with given name & argument.
        tpair<str, str> subAndOwner(const std::string& name, const ucontainable& args) {
            scopes& chn = subs().cast<scopes>();
            if(nul(chn)) return tpair<str, str>();

            for(scopes* e=&chn; e ;e=&e->getNext()) {
                node& owner = _getOwnerFrom(*e);
                if(nul(owner)) {
                    WRD_W("couldn't find owner from chain[%x]", &e.get());
                    continue;
                }

                node& ret = owner.sub(name, args);
                if(nul(ret)) continue;

                return tpair<str, str>(str(owner), str(ret));
            }

            return tpair<str, str>();
        }*/

        wbool pushLocal(nbicontainer* con);
        wbool pushLocal(nbicontainer& con);
        wbool pushLocal(scopes* new1);
        wbool pushLocal(scopes& new1);
        wbool pushLocal(const std::string& name, node& n);
        wbool pushLocal(const std::string& name, node* n);

        scopes& getTop();
        const scopes& getTop() const WRD_UNCONST_FUNC(getTop())

        tstr<scopes> popLocal();
        // I won't provide API for poping a single node from the scope.

        void setObj(const baseObj& new1);

        const baseObj& getObj() const;
        baseObj& getObj();

        void setFunc(func& new1);
        func& getFunc();
        const func& getFunc() const WRD_UNCONST_FUNC(getFunc())

        void clearObj();

        // node:
        using node::subs;
        nbicontainer& subs() override;

        wbool canRun(const ucontainable& args) const override;
        str run(const ucontainable& args) override;

        void rel() override;

        wbool pushReturn(const str& toReturn);
        wbool isReturned() const;
        str popReturn();

    private:
        void _rel();

    private:
        tstr<baseObj> _obj;
        tstr<func> _func;
        scopeStack _local;
        str _ret;
    };
}
