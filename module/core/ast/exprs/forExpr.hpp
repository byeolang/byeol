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
        /**
         * @brief Protected virtual method to create the actual loop mechanism.
         * @details This method is responsible for setting up the iteration logic for the 'for' loop.
         * @param ret A pointer to an array (`arr`) to store the results of the loop iterations (if applicable).
         * @return A `tstr` to a `loop` object that manages the loop's execution.
         */
        tstr<loop> _makeLoop(arr* ret) const override;
        /**
         * @brief Protected virtual method to create the evaluation part of the loop.
         * @details This method defines how the loop's body is evaluated during each iteration.
         * @return An `str` object representing the evaluated loop body.
         */
        str _makeEval() const override;

    private:
        str _container;
        std::string _name;
    };
}
