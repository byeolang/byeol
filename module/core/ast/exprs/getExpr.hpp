#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/params.hpp"

namespace by {

    class visitor;
    class closure;

    class _nout getExpr: public expr {
        NM(CLASS(getExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class parser;
        friend class visitor;     // for _me
        friend class generalizer; // for _args
        friend class closure;     // for `_evalMe()`

    public:
        getExpr(const std::string& name);
        getExpr(const std::string& name, const args& a);
        getExpr(const node& me, const std::string& name);
        getExpr(const node& me, const std::string& name, const args& a);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const node* getMe() const;
        const std::string& getName() const;
        void setName(const std::string& new1);
        void setName(const std::string* it) BY_SIDE_FUNC(setName)
        void setMe(const node& newMe);

        /// @return nullable
        const args& getArgs() const;
        void setArgs(const args& new1);

        void onCloneDeep(const clonable& new1) override;

    protected:
        virtual node* _onGet(node& me) const;
        virtual node* _onGet(node* it) const BY_SIDE_FUNC(it, _onGet(*it), nullptr);

    private:
        str _get(nbool evalMode) const;
        str _evalMe(nbool evalMode) const;

    private:
        str _me;
        std::string _name;
        tstr<args> _args;
    };
} // namespace by
