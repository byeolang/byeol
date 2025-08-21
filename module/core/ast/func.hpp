/// @file
#pragma once

#include "core/ast/exprs/endExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/exprs/blockExpr.hpp"
#include "core/ast/params.hpp"
#include "core/ast/scope.hpp"
#include "core/type/funcMgdType.hpp"

namespace by {

    class visitor;

    class _nout func: public baseFunc {
        BY(ME(func, baseFunc), CLONE(func), VISIT())
        friend class verifier;
        friend class exprMaker;
        friend class parser;
        typedef std::function<void(const std::string&, const node&)> onEval;

    public:
        explicit func(const modifier& mod, const funcMgdType& type);
        explicit func(const modifier& mod, const funcMgdType& type, const blockExpr& newBlock);
        explicit func(const modifier& mod, const funcMgdType& type, const scope& subs, const blockExpr& newBlock);

    public:
        const ntype& getType() const override;

        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        void setBlock(const blockExpr& new1);
        scope& subs() override;

        using super::run;
        str run(const args& a) override;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;

        using super::outFrame;
        void outFrame() const override;

        const baseObj& getOrigin() const override;

        ends& getEnds();
        const ends& getEnds() const BY_CONST_FUNC(getEnds())

        void onCloneDeep(const clonable& from) override;

        nbool isAbstract() const;

    protected:
        virtual str _interactFrame(node& meObj, scope& s, nidx exN);
        str _run(nidx exN);

    private:
        scope* _evalArgs(const args& args);
        str _postprocess(const str& ret, nidx exN);
        void _runEnds();
        void _setOrigin(const baseObj& org);

    public:
        inline static const std::string LAMBDA_NAME = "@lambda";

    private:
        tstr<obj> _org;
        funcMgdType _type;
        scope _subs;
        tstr<blockExpr> _blk;
        ends _ends;
    };
} // namespace by
