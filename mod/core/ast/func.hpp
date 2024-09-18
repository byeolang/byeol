#pragma once

#include "ast/exprs/endExpr.hpp"
#include "baseFunc.hpp"
#include "exprs/blockExpr.hpp"
#include "params.hpp"
#include "scope.hpp"

namespace nm {

    class visitor;

    class _nout func: public baseFunc {
        NM(ME(func, baseFunc), CLONE(func), VISIT())
        friend class verifier;
        friend class exprMaker;

    public:
        explicit func(const modifier& mod, const params& ps, const node& retType);
        explicit func(const modifier& mod, const params& ps, const node* retType);
        explicit func(const modifier& mod, const params& ps, const node& retType,
            const blockExpr& newBlock);
        explicit func(const modifier& mod, const params& ps, const node* retType,
            const blockExpr& newBlock);

    protected:
        explicit func(const modifier& mod, const mgdType& newType, const node& retType,
            const blockExpr& newBlock);

    public:
        const ntype& getType() const override;

        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        str getRet() const override;
        nbool setRet(const node& newRet) override;
        scope& subs() override;

        using super::run;
        str run(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        using super::outFrame;
        void outFrame(const bicontainable& args) override;

        ends& getEnds();
        const ends& getEnds() const NM_CONST_FUNC(getEnds())

        void onCloneDeep(const clonable& from) override;

    private:
        scope* _evalArgs(const ucontainable& args);
        str _postprocess(str ret, nidx exN);
        void _runEnds();

    private:
        mgdType _type;
        scope _shares;
        str _retType;
        tstr<blockExpr> _blk;
        ends _ends;
    };
} // namespace nm
