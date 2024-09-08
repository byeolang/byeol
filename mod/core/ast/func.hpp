#pragma once

#include "ast/exprs/endExpr.hpp"
#include "baseFunc.hpp"
#include "exprs/blockExpr.hpp"
#include "params.hpp"
#include "scope.hpp"

namespace nm {

    class visitor;

    class _nout func: public baseFunc {
        NM(CLASS(func, baseFunc), VISIT())
        friend class verifier;
        friend class exprMaker;

    public:
        explicit func(const modifier& mod, const params& ps, const node& retType);
        explicit func(const modifier& mod, const params& ps, const node* retType);
        explicit func(const modifier& mod, const params& ps, const node& retType,
            const blockExpr& newBlock);
        explicit func(const modifier& mod, const params& ps, const node* retType,
            const blockExpr& newBlock);

    public:
        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        str getRet() const override;
        nbool setRet(const node& newRet) override;
        scope& subs() override;
        const params& getParams() const override;

        using super::run;
        str run(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        using super::outFrame;
        void outFrame(const bicontainable& args) override;

        ends& getEnds();
        const ends& getEnds() const NM_CONST_FUNC(getEnds())

        clonable* cloneDeep() const override;

    private:
        scope* _evalArgs(const ucontainable& args);
        str _postprocess(str ret, nidx exN);
        void _runEnds();

    private:
        params _params;
        scope _shares;
        str _retType;
        tstr<blockExpr> _blk;
        ends _ends;
    };
} // namespace nm
