#pragma once

#include "../ast/baseFunc.hpp"
#include "../ast/baseObj.hpp"
#include "../ast/tmock.hpp"
#include "../builtin/res/tpair.hpp"
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

    class _nout frame: public node, public dumpable { // TODO: may be obj, not node.
        NM(CLASS(frame, node), VISIT())
        friend class verifier;
        friend struct ::frameTest;
        friend class baseObj;

    public:
        frame();
        ~frame() override;

        /// @param existing don't need to cloneChain() before passing this func.
        void add(const scope& existing);
        void add(const nbicontainer& existing);
        void add(const node& owner);
        virtual void add(const node& owner, const scope& s);
        virtual void addLocal(const std::string& name, const node& n);

        virtual void del();

        virtual nbool setMe(const node& obj); // 'me' can be a mockNode during verification.
        void setMe();
        const node& getMe() const NM_CONST_FUNC(getMe())
        node& getMe();

        scope& getLocals();
        const scope& getLocals() const NM_CONST_FUNC(getLocals())

        virtual nbool setFunc(const baseFunc& new1);
        void setFunc();
        baseFunc& getFunc();
        const baseFunc& getFunc() const NM_CONST_FUNC(getFunc())

        node& getOwner(const node& sub);
        const node& getOwner(const node& sub) const NM_CONST_FUNC(getOwner(sub))

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
        const scopeRegister& _getTop() const NM_CONST_FUNC(_getTop())

    private:
        str _me;
        tstr<baseFunc> _func;
        std::vector<scopeRegister> _stack;
        mutable str _ret;
    };
} // namespace nm
