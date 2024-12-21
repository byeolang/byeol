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
        friend class parser;
        typedef std::function<void(const std::string&, const node&)> onEval;

    public:
        explicit func(const modifier& mod, const mgdType& type);
        explicit func(const modifier& mod, const mgdType& type, const blockExpr& newBlock);
        explicit func(const modifier& mod, const mgdType& type, const scope& subs,
            const blockExpr& newBlock);

    public:
        const ntype& getType() const override;

        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        scope& subs() override;

        using super::run;
        str run(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable& args) const override;
        using super::outFrame;
        void outFrame(const bicontainable& args) const override;

        const baseObj& getOrigin() const override;

        ends& getEnds();
        const ends& getEnds() const NM_CONST_FUNC(getEnds())

        void onCloneDeep(const clonable& from) override;

        static void evalArgs(const ucontainable& args, const params& ps, onEval lambda);

        nbool isAbstract() const;

    protected:
        virtual str _interactFrame(node& meObj, scope& s, nidx exN);
        str _run(nidx exN);

    private:
        scope* _evalArgs(const ucontainable& args);
        str _postprocess(const str& ret, nidx exN);
        void _runEnds();
        void _setOrigin(const baseObj& org);

    public:
        inline static const std::string LAMBDA_NAME = "@lambda";

    private:
        tstr<obj> _org;
        mgdType _type;
        scope _subs;
        tstr<blockExpr> _blk;
        ends _ends;
    };
} // namespace nm
