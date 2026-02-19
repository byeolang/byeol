/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /** @ingroup core
     *  @brief Type casting expression
     *  @details Represents explicit type casting operations (e.g., value as Type).
     *  Handles type conversion and casting validation at runtime.
     */
    class _nout asExpr: public expr {
        BY(CLASS(asExpr, expr, expr::exprType), VISIT())

    public:
        asExpr(const node& me, const node& as);

    public:
        using super::eval;
        str eval(const args& a) override;
        str infer() const override;
        const node& getMe() const;
        const node& getAs() const;

        /**
         * @brief Sets the target type for the explicit type casting expression.
         * @param new1 The node representing the new target type.
         */
        void setAs(const node& new1);
        void setAs(const node* it) BY_SIDE_FUNC(setAs);

        void onCloneDeep(const clonable& from) override;

    private:
        str _me;
        str _as;
    };
}
