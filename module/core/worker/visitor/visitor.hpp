/// @file
#pragma once

#include "core/ast/node.hpp"
#include "core/ast/src.hpp"
#include "core/worker/tworker.hpp"
#include "core/worker/visitor/visitInfo.hpp"

namespace by {

    class retStateExpr;
#define X(T) class T;
#include "visitee.inl"
#undef X

    /** @ingroup core
     *  @brief Base visitor class for AST traversal
     *  @details Since byeol focuses on AST, visitor is used frequently. visitor is actively utilized to separate the
     *  traversal method from the actions taken when visiting @ref node during traversal.
     *
     *  @section traversal Traversal
     *  Always follows preorder traversal. Changing to postorder traversal is not possible. visit() consists of 3
     *  stages:
     *  1. Visit the currently found node (onVisit())
     *  2. Traverse next child nodes (onTraverse())
     *  3. Leave the currently found node (onLeave())
     *
     *  @section downcasting_through_accept Downcasting Through accept
     *  visitor has many virtual functions that represent visits to many concrete types like onVisit(T&). On the other
     *  hand, when searching in onTraverse, the node type is mainly used because it uses the @ref tbicontainable
     *  interface through node's `subs()`. So somewhere the node type must be downcast to concrete types like @ref nInt
     *  or @ref defNestedFuncExpr. For this purpose, node's virtual function `accept()` is called. See the example:
     *
     *  @code
     *      void defNestedFuncExpr::accept(const visitInfo& i, visitor& v) {
     *          v.visit(i, *this); // calls visitor::visit(const visitInfo&, defNestedFuncExpr&)
     *      }
     *  @endcode
     *
     *  When the virtual function accept() is called, it reversely calls visitor's visit() as a concrete type through
     *  *this. For this, all node-derived classes participating in visitation must override the virtual function
     *  `accept()`, and the VISIT macro is used to make this process easier. You will often see declarations like this:
     *
     *  @code
     *      class _nout slot: public node {
     *          BY(CLASS(slot, node), VISIT()) // <---
     *
     *      public:
     *  @endcode
     *
     *  If a node-derived class does not override `accept()`, `onTraverse(node&)` is used instead, which is sufficient
     *  for those cases.
     *
     *  @section duplicate_visit_elimination Duplicate Visit Elimination
     *  AST sometimes has mutual references between nodes. In this case, traversing without any exception handling
     *  revisits already visited nodes and enters an infinite loop. visitor owns a map called `_visited`. Through this,
     *  when visit() is called, it determines if it's an already visited node and provides exception handling. This
     *  visit history information is reset right before visitor starts visiting each time. If you want to enable
     *  revisiting, change the value with `setReturnable(true)`.
     */
    class _nout visitor: public tworker<void, node> {
        typedef tworker<void, node> __super6;
        BY(CLASS(visitor, __super6))

    public:
        visitor();
        visitor(nbool isReturnable);

    public:
        /**
         *  if you set the visitor as returnable, nodes you have already been visited,
         *  will be visited again if it's refered by different nodes.
         *  in default, this value is false.
         */
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
        /**
         *  @return false if the node is already visited.
         */
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
