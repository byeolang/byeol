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
        virtual void visit(const visitInfo& i, T& me); \
        virtual nbool onVisit(const visitInfo& i, T& me); \
        virtual void onLeave(const visitInfo& i, T& me);
#   include "visitee.inl"
#undef X

        virtual void visit(const visitInfo& i, node& me);
        virtual nbool onVisit(const visitInfo& i, node& me);
        virtual void onLeave(const visitInfo& i, node& me);

        //  traverse:
        //      generalized-way:
        //  @ return false if the node is already visited.
        virtual void onTraverse(const visitInfo& i, node& me);
        //      specific-way:
        virtual void onTraverse(const visitInfo& i, asExpr& a);
        virtual void onTraverse(const visitInfo& i, assignExpr& a);
        virtual void onTraverse(const visitInfo& i, blockExpr& b);
        virtual void onTraverse(const visitInfo& i, defVarExpr& d);
        virtual void onTraverse(const visitInfo& i, FBOExpr& f);
        virtual void onTraverse(const visitInfo& i, getExpr& e);
        virtual void onTraverse(const visitInfo& i, retExpr& b);
        virtual void onTraverse(const visitInfo& i, runExpr& e);
        virtual void onTraverse(runExpr& e, node& subject);
        virtual void onTraverse(const visitInfo& i, func& f);
        virtual void onTraverse(const visitInfo& i, frame& f);
        virtual void onTraverse(const visitInfo& i, forExpr& f);
        virtual void onTraverse(const visitInfo& i, retStateExpr& r);
        virtual void onTraverse(const visitInfo& i, ifExpr& f);
        virtual void onTraverse(const visitInfo& i, whileExpr& w);
        virtual void onTraverse(const visitInfo& i, defArrayExpr& d);
        virtual void onTraverse(const visitInfo& i, genericOrigin& g);

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
