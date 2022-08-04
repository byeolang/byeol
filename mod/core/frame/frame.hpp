#pragma once

#include "../ast/baseObj.hpp"
#include "scopeStack.hpp"
#include "../builtin/res/tpair.hpp"

struct immutableTest;
struct frameTest;

namespace namu {

    class obj;
    class func;
    class _nout frame : public node { // TODO: may be obj, not node.
        NAMU(CLASS(frame, node),
            FRIEND_VERIFY(baseObj, subNodes))
        friend struct ::immutableTest;
        friend struct ::frameTest;
        friend class baseObj;

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
                    NAMU_W("couldn't find owner from chain[%x]", &e.get());
                    continue;
                }

                node& ret = owner.sub(name, args);
                if(nul(ret)) continue;

                return tpair<str, str>(str(owner), str(ret));
            }

            return tpair<str, str>();
        }*/

        nbool pushLocal(nbicontainer* con);
        nbool pushLocal(nbicontainer& con);
        nbool pushLocal(scopes* new1);
        nbool pushLocal(scopes& new1);
        nbool pushLocal(const std::string& name, const node& n);
        nbool pushLocal(const std::string& name, const node* n);
        void pushObj(const baseObj& obj);

        scopes& getTop();
        const scopes& getTop() const NAMU_UNCONST_FUNC(getTop())

        // I won't provide API for poping a single node from the scope.
        tstr<scopes> popLocal();

        static baseObj& getObj();

        void setFunc(func& new1);
        func& getFunc();
        const func& getFunc() const NAMU_UNCONST_FUNC(getFunc())

        // node:
        using node::subs;
        nbicontainer& subs() override;

        nbool canRun(const ucontainable& args) const override;
        str run(const ucontainable& args) override;

        void rel() override;

        nbool pushReturn(const str& toReturn);
        nbool isReturned() const;
        str popReturn();

    protected:
        static baseObj& _setObj(baseObj& new1);
        static baseObj& _setObj();

    private:
        void _rel();

    private:
        tstr<baseObj> _obj;
        tstr<func> _func;
        scopeStack _local;
        str _ret;
    };
}
