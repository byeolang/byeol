#pragma once

#include "core/ast/exprs/loopExpr.hpp"

namespace nm {

    class frame;

    class _nout forExpr: public loopExpr {
        NM(CLASS(forExpr, loopExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        forExpr(const std::string& localName, const node& container, const blockExpr& blk);

    public:
        const std::string& getLocalName() const;
        str getContainer();

    protected:
        tstr<loop> _makeLoop(arr& ret) const override;
        str _makeEval() const override;

    private:
        str _container;
        std::string _name;
    };
}
