#pragma once

#include "../ast/baseObj.hpp"
#include "../ast/baseFunc.hpp"
#include "scopeStack.hpp"
#include "../builtin/res/tpair.hpp"
#include "../ast/mockNode.hpp"

struct immutableTest;
struct frameTest;

namespace namu {
    class obj;
    class baseFunc;
    class _nout frame : public node { // TODO: may be obj, not node.
        NAMU(CLASS(frame, node), VISIT())
        friend class verifier;
        friend struct ::immutableTest;
        friend struct ::frameTest;
        friend class baseObj;

    public:
        frame();
        ~frame() override;

        /* TODO: can I remove this?:

        /// @return returns tpair<node, itsOwner>. the owner has the container holding the node
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

        node& getObjHaving(const node& sub);
        const node& getObjHaving(const node& sub) const NAMU_UNCONST_FUNC(getObjHaving(sub))

        nbicontainer& getTop();
        const nbicontainer& getTop() const NAMU_UNCONST_FUNC(getTop())

        // I won't provide API for poping a single node from the scope.
        tstr<scopes> popLocal();

        void setFunc(baseFunc& new1);
        void setFunc();
        baseFunc& getFunc();
        const baseFunc& getFunc() const NAMU_UNCONST_FUNC(getFunc())
        const baseObj& getMe() const NAMU_UNCONST_FUNC(getMe())
        baseObj& getMe();

        // node:
        using node::subs;
        nbicontainer& subs() override;

        priority prioritize(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        nbool setRet(const node& newRet) const;
        nbool setRet() const;
        node& getRet() const;

    protected:
        static baseObj& _setMe(baseObj& new1);
        static baseObj& _setMe();
        static baseObj& _getMe();

    private:
        void _rel();

    private:
        tstr<baseObj> _obj;
        tstr<baseFunc> _func;
        scopeStack _local;
        mutable str _ret;
    };
}
