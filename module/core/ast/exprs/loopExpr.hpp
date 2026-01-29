/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/exprs/blockExpr.hpp"

namespace by {

    class frame;
    class arr;

    /** @ingroup core
     *  @brief Base class for loop expressions
     *  @details Abstract base class for all loop constructs (for, while).
     *  Provides common loop execution logic and result collection.
     */
    class _nout loopExpr: public expr {
        BY(ADT(loopExpr, expr, expr::exprType), VISIT())
        friend class loop;

    public:
        /** @ingroup core
         *  @brief Loop execution controller
         *  @details Manages loop iteration state and execution flow.
         */
        class _nout loop: public instance {
            BY(ADT(loop))

        public:
            loop(arr* ret);

        public:
            /**
             *  @return false if exit the loop
             */
            virtual nbool isLooping() = 0;
            /**
             * @brief Evaluates the loop's block expression within the given frame.
             * @param blk The block expression representing the loop body.
             * @param fr The current execution frame.
             */
            virtual void eval(blockExpr& blk, frame& fr);
            arr* getRet();
            /**
             * @brief Performs post-processing after each loop iteration or upon loop completion.
             * @param fr The current execution frame.
             * @return true to continue post-processing, false to stop.
             */
            virtual nbool postprocess(frame& fr);


        private:
            tstr<arr> _ret;
        };

    public:
        loopExpr(const blockExpr& blk);

    public:
        blockExpr* getBlock() const;
        str infer() const override;

        using super::eval;
        str eval(const args& a) override;

    protected:
        /**
         * @brief Protected pure virtual method to create the loop controller.
         * @details Derived classes must implement this to define how the specific loop construct
         *          (e.g., for loop, while loop) manages its iterations.
         * @param ret A pointer to an array (`arr`) to store the results of the loop iterations (if applicable).
         * @return A `tstr` to a `loop` object that manages the loop's execution.
         */
        virtual tstr<loop> _makeLoop(arr* ret) const = 0;
        /**
         * @brief Protected virtual method to create the evaluation block for the loop body.
         * @details This method is responsible for setting up how the loop's body will be evaluated
         *          during each iteration.
         * @return An `str` object representing the evaluated loop body.
         */
        virtual str _makeEval() const;

    private:
        tstr<arr> _makeRet() const;

    private:
        mutable tstr<blockExpr> _blk;
        mutable str _eval;
    };
} // namespace by
