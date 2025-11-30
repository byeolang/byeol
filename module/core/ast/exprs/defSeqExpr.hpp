/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief @ref seq definition expression
    /// @details Represents sequence range definitions (e.g., start..end).
    /// Creates integer sequences with specified start and end values.
    class _nout defSeqExpr: public expr {
        BY(CLASS(defSeqExpr, expr, expr::exprType), VISIT())
        friend class func;
        friend class verifier;

    public:
        defSeqExpr(const node& start, const node& end);

    public:
        using super::run;
        str run(const args& a) override;

        str infer() const override;

        const node& getStart() const;
        const node& getEnd() const;

    private:
        str _start;
        str _end;
    };
}
