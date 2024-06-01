#pragma once

#include "baseFunc.hpp"
#include "exprs/blockExpr.hpp"
#include "../builtin/container/native/tnmap.hpp"
#include "params.hpp"

namespace namu {

    class visitor;
    class scope;

    class _nout func : public baseFunc {
        NAMU(CLASS(func, baseFunc), VISIT())
        friend class verifier;
        friend class exprMaker;

    public:
        explicit func(const params& ps, const node& retType);
        explicit func(const params& ps, const node* retType);
        explicit func(const params& ps, const node& retType, const blockExpr& newBlock);
        explicit func(const params& ps, const node* retType, const blockExpr& newBlock);

    public:
        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        str getRet() const override;
        nbool setRet(const node& newRet) override;
        nbicontainer& subs() override;
        const params& getParams() const override;

        using super::run;
        str run(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        void outFrame() override;

        clonable* cloneDeep() const override;

    private:
        scope* _evalArgs(const ucontainable& args);
        str _postprocess(str ret, nidx exN);

    private:
        params _params;
        nmap _shares;
        str _retType;
        tstr<blockExpr> _blk;
    };
}
