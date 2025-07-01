#pragma once

#include "core/ast/exprs/getExpr.hpp"

namespace nm {

    class visitor;
    class genericOrigin;

    class _nout getGenericExpr: public getExpr {
        NM(CLASS(getGenericExpr, getExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        getGenericExpr(const std::string& genericName, const args& typeParams);
        getGenericExpr(const node& me, const std::string& genericName, const args& typeParams);

    protected:
        node* _onGet(node& me) const override;
    };
}
