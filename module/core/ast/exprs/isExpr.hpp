/// @file
#pragma once

#include "core/ast/expr.hpp"

namespace by {
    /// @ingroup core
    /// @brief Type checking expression
    /// @details Represents type checking operations (e.g., value is Type).
    /// Evaluates to boolean indicating whether an object is of specified type.
    class _nout isExpr: public expr {
        BY(CLASS(isExpr, expr, expr::exprType), VISIT())

    public:
        isExpr(const node& me, const node& to);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;
        const node& getMe() const;
        const node& getTo() const;
        void setTo(const node& new1);
        void setTo(const node* it) BY_SIDE_FUNC(setTo);

    private:
        str _me;
        str _to;
    };
}
