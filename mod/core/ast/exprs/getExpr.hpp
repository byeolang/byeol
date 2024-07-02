#pragma once

#include "../expr.hpp"
#include "../params.hpp"

namespace nm {

    class visitor;

    class _nout getExpr : public expr {
        NAMU(CLASS(getExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class parser;
        friend class visitor; // for _me
        friend class generalizer; // for _args

    public:
        getExpr(const std::string& name);
        getExpr(const std::string& name, const args& a);
        getExpr(const node& me, const std::string& name);
        getExpr(const node& me, const std::string& name, const args& a);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getMe() const;
        const std::string& getName() const;
        void setName(const std::string& new1);
        void setMe(const node& newMe);

        /// @return nullable
        const args& getArgs() const;
        void setArgs(const args& new1);

        clonable* cloneDeep() const override;

    protected:
        virtual priorities _get(nbool evalMode) const;

    private:
        str _me;
        std::string _name;
        tstr<args> _args;
    };
}