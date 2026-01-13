/// @file
#pragma once

#include "core/ast/exprs/loopExpr.hpp"

namespace by {

    class frame;

    /** @ingroup core
     *  @brief For loop expression
     *  @details Represents for-in loop constructs for iterating over containers.
     *  Manages iterator variable binding and loop body execution.
     */
    class _nout forExpr: public loopExpr {
        BY(CLASS(forExpr, loopExpr, expr::exprType), VISIT())
        friend class verifier;

    public:
        forExpr(const std::string& localName, const node& container, const blockExpr& blk);

    public:
        const std::string& getLocalName() const;
        str getContainer();

    protected:
        tstr<loop> _makeLoop(arr* ret) const override;
        str _makeEval() const override;

    private:
        str _container;
        std::string _name;
    };
}
