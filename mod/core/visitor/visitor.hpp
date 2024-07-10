#pragma once

#include "visitInfo.hpp"
#include "../loader/worker/worker.hpp"
#include "../ast/node.hpp"
#include "../ast/src.hpp"

namespace nm {

    class retStateExpr;
#define X(T) class T;
#   include "visitee.inl"
#undef X

    class _nout visitor : public worker<void, node> {
        typedef worker<void, node> __super6;
        NM(CLASS(visitor, __super6))

    public:
#define X(T) \
        virtual void visit(visitInfo i, T& me); \
        virtual nbool onVisit(visitInfo i, T& me); \
        virtual void onLeave(visitInfo i, T& me);
#   include "visitee.inl"
#undef X

        virtual void visit(visitInfo i, node& me);
        virtual nbool onVisit(visitInfo i, node& me);
        virtual void onLeave(visitInfo i, node& me);

        //  traverse:
        //      generalized-way:
        //  @ return false if the node is already visited.
        virtual void onTraverse(visitInfo i, node& me);
        //      specific-way:
        virtual void onTraverse(visitInfo i, asExpr& a);
        virtual void onTraverse(visitInfo i, assignExpr& a);
        virtual void onTraverse(visitInfo i, blockExpr& b);
        virtual void onTraverse(visitInfo i, defVarExpr& d);
        virtual void onTraverse(visitInfo i, FBOExpr& f);
        virtual void onTraverse(visitInfo i, getExpr& e);
        virtual void onTraverse(visitInfo i, retExpr& b);
        virtual void onTraverse(visitInfo i, runExpr& e);
        virtual void onTraverse(runExpr& e, node& subject);
        virtual void onTraverse(visitInfo i, func& f);
        virtual void onTraverse(visitInfo i, frame& f);
        virtual void onTraverse(visitInfo i, forExpr& f);
        virtual void onTraverse(visitInfo i, retStateExpr& r);
        virtual void onTraverse(visitInfo i, ifExpr& f);
        virtual void onTraverse(visitInfo i, whileExpr& w);
        virtual void onTraverse(visitInfo i, defArrayExpr& d);
        virtual void onTraverse(visitInfo i, genericObj& g);

        virtual void onTraverse(ifExpr& e, blockExpr& blk);

        using super::warn;
        template <typename... Args>
        void posWarn(errCode code, const node& it, const Args&... args) {
            _report(err::newWarn(it.getSrc().getPos(), code, __convert__((const Args&) args).unwrap()...));
        }

        template <typename... Args>
        void posError(errCode code, const node& it, const Args&... args) {
            _report(err::newErr(it.getSrc().getPos(), code, __convert__((const Args&) args).unwrap()...));
        }

        template <typename... Args>
        void posInfo(errCode code, const node& it, const Args&... args) {
            _report(err::newInfo(it.getSrc().getPos(), code, __convert__((const Args&) args).unwrap()...));
        }

    protected:
        void _onWork() override;
        void _prepare() override;

    private:
        /// @return false if the node is already visited.
        nbool _markVisited(node& me);
        void _rel();

    private:
        // value will be true if key is visited func or obj:
        //  obj usually has huge subs and was chained to its subpack. but subpack also has the
        //  obj as its one of subs, so visitor will fall in the infinite recursive loop.
        //  to prevent it, I prepare a map instance. that map will return true if the key,
        //  func or obj, already got visited.
        std::map<node*, nbool> _visited;
    };
}
