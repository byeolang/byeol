/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Sequence definition expression
     *  @details Handles sequence definition expressions.
     *
     *  @remark seq class
     *  @ref nseq is the class handling arrays in the native environment. @ref seq extends nseq for the managed
     *  environment. This allows similar APIs to be used in both C++ and byeol. See seq and nseq for details.
     */
    class _nout defSeqExpr: public expr {
        BY(CLASS(defSeqExpr, expr, expr::exprType), VISIT())
        friend class func;
        friend class verifier;

    public:
        defSeqExpr(const node& start, const node& end);

    public:
        using super::eval;
        str eval(const args& a) override;

        str infer() const override;

        const node& getStart() const;
        const node& getEnd() const;

    private:
        str _start;
        str _end;
    };
}
