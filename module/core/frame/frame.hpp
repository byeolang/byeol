/// @file
#pragma once

#include "core/ast/baseFunc.hpp"
#include "core/ast/baseObj.hpp"

struct immutableTest;
struct frameTest;

namespace by {

    /// @ingroup core
    /// @brief Scope registration entry for frame management
    /// @details Stores scope ownership information and linkage for frame stack management.
    struct _nout scopeRegister {
        str owner;
        tstr<scope> s;
        tstr<scope> linkedS; // linked scope to previous element of _stack.
    };

    class obj;
    class baseFunc;

    /// @ingroup core
    /// @brief Execution frame for byeol runtime
    /// @details Manages execution context including scopes, variables, function calls, and return values.
    /// Provides stack-based scope management for the byeol language runtime environment.
    class _nout frame: public node, public dumpable { // TODO: may be obj, not node.
        BY(CLASS(frame, node), VISIT())
        friend class verifier;
        friend struct ::frameTest;
        friend class baseObj;

    public:
        frame();
        ~frame() override;

        /// @param existing don't need to cloneChain() before passing this func.
        void add(const scope& existing);
        void add(const scope* it) BY_SIDE_FUNC(add);
        void add(const nbicontainer& existing);
        void add(const nbicontainer* it) BY_SIDE_FUNC(add);
        void add(const node& owner);
        void add(const node* it) BY_SIDE_FUNC(add);
        virtual void add(const node* owner, const scope& s);
        void add(const node* owner, const scope* s) BY_SIDE_FUNC(s, add(owner, *s), void());

        virtual void addLocal(const std::string& name, const node& n);
        void addLocal(const std::string* name, const node& n) BY_SIDE_FUNC(name, addLocal(*name, n), void());
        void addLocal(const std::string& name, const node* n) BY_SIDE_FUNC(n, addLocal(name, *n), void());
        void addLocal(const std::string* name, const node* n) BY_SIDE_FUNC(name&& n, addLocal(*name, *n), void());
        void addLocal(const nchar* name, const node& n) BY_SIDE_FUNC(name, addLocal(std::string(name), n), void());
        void addLocal(const nchar* name, const node* n) BY_SIDE_FUNC(name&& n, addLocal(std::string(name), *n), void());

        virtual void del();

        virtual nbool setMe(const node& obj); // 'me' can be a mockNode during verification.
        nbool setMe(const node* it) BY_SIDE_FUNC(setMe);
        void setMe();

        const node* getMe() const BY_CONST_FUNC(getMe())
        node* getMe();

        scope* getLocals();
        const scope* getLocals() const BY_CONST_FUNC(getLocals())

        virtual nbool addFunc(const baseFunc& new1);
        nbool addFunc(const baseFunc* it) BY_SIDE_FUNC(addFunc);

        void delFunc();

        baseFunc* getFunc();
        const baseFunc* getFunc() const BY_CONST_FUNC(getFunc())

        node* getMeHaving(const node& sub);
        node* getMeHaving(const node* it) BY_SIDE_FUNC(getMeHaving);
        const node* getMeHaving(const node& sub) const BY_CONST_FUNC(getMeHaving(sub))
        const node* getMeHaving(const node* sub) const BY_CONST_FUNC(getMeHaving(sub))

        scope* getScopeHaving(const node& sub);
        scope* getScopeHaving(const node* it) BY_SIDE_FUNC(getScopeHaving);
        const scope* getScopeHaving(const node& sub) const BY_CONST_FUNC(getScopeHaving(sub))
        const scope* getScopeHaving(const node* sub) const BY_CONST_FUNC(getScopeHaving(sub))

        // node:
        using node::subs;
        scope& subs() override;
        tstr<nbicontainer> mySubs() const override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        using super::eval;
        str eval(const args& a) override;

        void rel() override;

        const std::vector<scopeRegister>& getScopeRegisters() const;

        virtual nbool setRet(const node& newRet) const;
        virtual nbool setRet(const node* it) const;

        node* getRet() const;

        void dump() const override;

    private:
        void _rel();
        scopeRegister* _getTop();
        const scopeRegister* _getTop() const BY_CONST_FUNC(_getTop())
        void _dumpFunc() const;

        template <typename T> T* _getOwner(const node* toFind, std::function<T*(nbool, scopeRegister&)> cl);

    private:
        str _me;
        tnarr<baseFunc> _funcs;
        std::vector<scopeRegister> _stack;
        mutable str _ret;
    };
} // namespace by
