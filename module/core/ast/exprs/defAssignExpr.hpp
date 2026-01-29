/// @file
#pragma once

#include "core/ast/exprs/defVarExpr.hpp"

namespace by {
    /** @ingroup core
     *  @brief Variable definition with assignment expression
     *  @details Represents variable declarations with initial value assignment.
     *  Handles variable creation, type specification, and value initialization.
     */
    class _nout defAssignExpr: public defVarExpr {
        BY(CLASS(defAssignExpr, defVarExpr, expr::exprType), VISIT())

    public:
        defAssignExpr(const std::string& name, const node* rhs);
        defAssignExpr(const std::string& name, const node* rhs, const node* to, const src& s, const modifier& mod);
        defAssignExpr(const std::string& name, const node* type, const node* rhs, const node* to, const src& s,
            const modifier& mod);

    public:
        str infer() const override;
        const node* getExplicitType() const;
        /**
         * @brief Sets the explicitly declared type for the variable being defined.
         * @param newType The node representing the explicit type.
         */
        void setExplicitType(const node& newType);

    protected:
        str _onMakeNew() override;

    private:
        str _type;
    };
}
