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

        virtual nbool addFunc(const baseFunc& new1);
        void delFunc();
        baseFunc& getFunc();
        const baseFunc& getFunc() const NM_CONST_FUNC(getFunc())

        node& getMeHaving(const node& sub);
        const node& getMeHaving(const node& sub) const NM_CONST_FUNC(getMeHaving(sub))
        scope& getScopeHaving(const node& sub);
        const scope& getScopeHaving(const node& sub) const NM_CONST_FUNC(getScopeHaving(sub))

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

        template <typename T>
        T& _getOwner(const node& toFind, std::function<T*(nbool, scopeRegister&)> cl) {
            if(nul(toFind)) return nulOf<T>();

            [[maybe_unused]] const nchar* name = toFind.getType().getName().c_str();
            for(auto& reg: _stack) {
                nbool has = reg.s->in(toFind);
                NM_DI("`%s` is in `%s` scope? --> %s", name,
                    reg.owner ? reg.owner->getSrc().getName() : "{local}", has);
                T* ret = cl(has, reg);
                if(ret) return *ret;
            }

            NM_E("couldn't find owner of %s", toFind);
            return nulOf<T>();
        }

    private:
        str _me;
        tnarr<baseFunc> _funcs;
        std::vector<scopeRegister> _stack;
        mutable str _ret;
    };
} // namespace nm
