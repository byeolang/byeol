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
        friend struct ::frameTest;
        friend class baseObj;

    public:
        frame();
        ~frame() override;

        /// @param existing don't need to cloneChain() before passing this func.
        nbool add(scopes& existing);
        nbool addLocal(const std::string& name, const node& n);

        void del();

        nbool setMe(const baseObj& obj);
        void setMe();
        const baseObj& getMe() const NAMU_UNCONST_FUNC(getMe())
        baseObj& getMe();

        nbool setFunc(baseFunc& new1);
        void setFunc();
        baseFunc& getFunc();
        const baseFunc& getFunc() const NAMU_UNCONST_FUNC(getFunc())

        node& getObjHaving(const node& sub);
        const node& getObjHaving(const node& sub) const NAMU_UNCONST_FUNC(getObjHaving(sub))

        // node:
        using node::subs;
        nbicontainer& subs() override;

        priority prioritize(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        nbool setRet(const node& newRet) const;
        nbool setRet() const;
        node& getRet() const;

    private:
        void _rel();

    private:
        tstr<baseObj> _me;
        tstr<baseFunc> _func;
        tstr<scopes> _stack;
        mutable str _ret;
    };
}
