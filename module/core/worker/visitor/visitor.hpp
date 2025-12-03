/// @file
#pragma once

#include "core/ast/node.hpp"
#include "core/ast/src.hpp"
#include "core/worker/worker.hpp"
#include "core/worker/visitor/visitInfo.hpp"

namespace by {

    class retStateExpr;
#define X(T) class T;
#include "visitee.inl"
#undef X

    /// @ingroup core
    /// @brief Base visitor class for AST traversal
    /// @details Implements the visitor pattern for traversing and processing AST nodes.
    /// Provides visit/leave hooks and tracks visited nodes to prevent infinite recursion.
    class _nout visitor: public worker<void, node> {
        typedef worker<void, node> __super6;
        BY(CLASS(visitor, __super6))

    public:
        visitor();
        visitor(nbool isReturnable);

    public:
        /// if you set the visitor as returnable, nodes you have already been visited,
        /// will be visited again if it's refered by different nodes.
        /// in default, this value is false.
        void setReturnable(nbool isReturnable);
        nbool isReturnable() const;

#define X(T)                                                                \
    virtual void visit(const visitInfo& i, T& me);                          \
    virtual nbool onVisit(const visitInfo& i, T& me, nbool alreadyVisited); \
    virtual void onLeave(const visitInfo& i, T& me, nbool alreadyVisited);
#include "visitee.inl"
#undef X

        virtual void visit(const visitInfo& i, node& me);
        virtual nbool onVisit(const visitInfo& i, node& me, nbool alreadyVisited);
        virtual void onLeave(const visitInfo& i, node& me, nbool alreadyVisited);

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
        virtual void onTraverse(const visitInfo& i, evalExpr& e);
        virtual void onTraverse(evalExpr& e, node& subject);
        virtual void onTraverse(const visitInfo& i, func& f);
        virtual void onTraverse(const visitInfo& i, frame& f);
        virtual void onTraverse(const visitInfo& i, forExpr& f);
        virtual void onTraverse(const visitInfo& i, retStateExpr& r);
        virtual void onTraverse(const visitInfo& i, ifExpr& f);
        virtual void onTraverse(const visitInfo& i, whileExpr& w);
        virtual void onTraverse(const visitInfo& i, defArrayExpr& d);
        virtual void onTraverse(const visitInfo& i, defNestedFuncExpr& e);
        virtual void onTraverse(const visitInfo& i, genericOrigin& g);
        virtual void onTraverse(const visitInfo& i, obj& o);

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
        nbool _isReturnable;
    };
} // namespace by
