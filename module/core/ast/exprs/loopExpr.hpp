/// @file
#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/exprs/blockExpr.hpp"

namespace by {

    class frame;
    class arr;

    /// @ingroup core
    /// @brief Base class for loop expressions
    /// @details Abstract base class for all loop constructs (for, while).
    /// Provides common loop execution logic and result collection.
    class _nout loopExpr: public expr {
        BY(ADT(loopExpr, expr, expr::exprType), VISIT())
        friend class loop;

    public:
        /// @ingroup core
        /// @brief Loop execution controller
        /// @details Manages loop iteration state and execution flow.
        class _nout loop: public instance {
            BY(ADT(loop))

        public:
            loop(arr* ret);

        public:
            /// @return false if exit the loop
            virtual nbool isLooping() = 0;
            virtual void eval(blockExpr& blk, frame& fr);
            arr* getRet();
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
        virtual tstr<loop> _makeLoop(arr* ret) const = 0;
        virtual str _makeEval() const;

    private:
        tstr<arr> _makeRet() const;

    private:
        mutable tstr<blockExpr> _blk;
        mutable str _eval;
    };
} // namespace by
