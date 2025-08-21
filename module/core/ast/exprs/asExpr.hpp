/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/param.hpp"
#include "core/ast/scope.hpp"

namespace by {

    class visitor;

    /// @ingroup core
    /// @brief Type casting expression
    /// @details Represents explicit type casting operations (e.g., value as Type).
    /// Handles type conversion and casting validation at runtime.
    class _nout asExpr: public expr {
        BY(CLASS(asExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class func;

    public:
        asExpr(const node& me, const node& as);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getMe() const;
        const node& getAs() const;

        void setAs(const node& new1);
        void setAs(const node* it) BY_SIDE_FUNC(setAs);

        void onCloneDeep(const clonable& from) override;

    private:
        str _me;
        str _as;
    };
}
