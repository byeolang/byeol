#pragma once

#include "../expr.hpp"
#include "blockExpr.hpp"

namespace namu {

    class frame;
    class arr;
    class _nout loopExpr : public expr {
        NAMU(ADT(loopExpr, expr, expr::exprType), VISIT())
        friend class loop;

    public:
        class _nout loop : public instance {
            NAMU(ADT(loop))

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
        nbool setEval(const node& new1) const;

        using super::run;
        str run(const args& a) override;

    protected:
        virtual tstr<loop> _makeLoop(arr& ret) const = 0;

    private:
        tstr<arr> _makeRet() const;

    private:
        mutable tstr<blockExpr> _blk;
        mutable str _eval;
    };
}
