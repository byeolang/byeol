#pragma once

#include "../ast/baseObj.hpp"
#include "../ast/baseFunc.hpp"
#include "../builtin/res/tpair.hpp"
#include "../ast/mockNode.hpp"
#include "../type/dumpable.hpp"

struct immutableTest;
struct frameTest;

namespace nm {

    struct _nout scopeRegister {
        str owner;
        tstr<scope> s;
    };

    class obj;
    class baseFunc;
    class _nout frame : public node, public dumpable { // TODO: may be obj, not node.
        NAMU(CLASS(frame, node), VISIT())
        friend class verifier;
        friend struct ::frameTest;
        friend class baseObj;

    public:
        frame();
        ~frame() override;

        /// @param existing don't need to cloneChain() before passing this func.
        void add(scope& existing);
        void add(nbicontainer& existing);
        void add(node& owner);
        virtual void add(node& owner, scope& s);
        virtual void addLocal(const std::string& name, const node& n);

        virtual void del();

        virtual nbool setMe(const baseObj& obj);
        void setMe();
        const baseObj& getMe() const NAMU_UNCONST_FUNC(getMe())
        baseObj& getMe();

        scope& getLocals();
        const scope& getLocals() const NAMU_UNCONST_FUNC(getLocals())

        virtual nbool setFunc(baseFunc& new1);
        void setFunc();
        baseFunc& getFunc();
        const baseFunc& getFunc() const NAMU_UNCONST_FUNC(getFunc())

        node& getOwner(const node& sub);
        const node& getOwner(const node& sub) const NAMU_UNCONST_FUNC(getOwner(sub))

        // node:
        using node::subs;
        scope& subs() override;
        tstr<nbicontainer> mySubs() const override;

        priorType prioritize(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        const std::vector<scopeRegister>& getScopeRegisters() const;

        virtual nbool setRet(const node& newRet) const;
        nbool setRet() const;
        node& getRet() const;

        void dump() const override;

    private:
        void _rel();
        scopeRegister& _getTop();
        const scopeRegister& _getTop() const NAMU_UNCONST_FUNC(_getTop())

    private:
        tstr<baseObj> _me;
        tstr<baseFunc> _func;
        std::vector<scopeRegister> _stack;
        mutable str _ret;
    };
}
