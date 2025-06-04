#pragma once

#include "core/ast/expr.hpp"
#include "core/ast/exprs/blockExpr.hpp"

namespace nm {

    class frame;
    class arr;

    class _nout loopExpr: public expr {
        NM(ADT(loopExpr, expr, expr::exprType), VISIT())
        friend class loop;

    public:
        class _nout loop: public instance {
            NM(ADT(loop))

        public:
            loop(arr& ret);

        public:
            /// @return false if exit the loop
            virtual nbool isLooping() = 0;
            virtual void run(blockExpr& blk, frame& fr);
            arr& getRet();
            virtual nbool postprocess(frame& fr);


        private:
            tstr<arr> _ret;
        };

    public:
        loopExpr(const blockExpr& blk);

    public:
        blockExpr& getBlock() const;
        str getEval() const override;

        using super::run;
        str run(const args& a) override;

    protected:
        virtual tstr<loop> _makeLoop(arr& ret) const = 0;
        virtual str _makeEval() const;

    private:
        tstr<arr> _makeRet() const;

    private:
        mutable tstr<blockExpr> _blk;
        mutable str _eval;
    };
} // namespace nm
