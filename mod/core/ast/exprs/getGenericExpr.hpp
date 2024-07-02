#pragma once

#include "getExpr.hpp"

namespace nm {

    class visitor;
    class genericObj;

    class _nout getGenericExpr : public getExpr {
        NAMU(CLASS(getGenericExpr, getExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        getGenericExpr(const std::string& genericName, const args& typeParams);
        getGenericExpr(const node& me, const std::string& genericName, const args& typeParams);

    protected:
        genericObj& _getGenericObj() const;

    protected:
        priorities _get(nbool evalMode) const override;
    };
}
