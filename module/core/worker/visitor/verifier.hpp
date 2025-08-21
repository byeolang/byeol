/// @file
#pragma once

#include "core/worker/visitor/visitor.hpp"
#include "core/ast/params.hpp"
#include "core/frame/frame.hpp"

struct verifierTest;

namespace by {

    class loopExpr;
    class thread;
    class endExpr;

    /// @ingroup core
    /// @brief AST verifier for semantic analysis
    /// @details Performs semantic analysis and type checking on AST nodes.
    /// Validates code correctness, type compatibility, and language semantics.
    class _nout verifier: public visitor {
        BY(CLASS(verifier, visitor))
        friend struct ::verifierTest;
        friend class genericOrigin;

    public:
        using super::onLeave;
        using super::onVisit;
        void onLeave(const visitInfo& i, node& me, nbool) override;
        void onLeave(const visitInfo& i, asExpr& me, nbool) override;
        void onLeave(const visitInfo& i, isExpr& me, nbool) override;
        void onLeave(const visitInfo& i, assignExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool) override;
        void onLeave(const visitInfo& i, blockExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defVarExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defPropExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defAssignExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defSeqExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defArrayExpr& me, nbool) override;
        void onLeave(const visitInfo& i, defNestedFuncExpr& me, nbool) override;
        void onLeave(const visitInfo& i, FBOExpr& me, nbool) override;
        void onLeave(const visitInfo& i, FUOExpr& me, nbool) override;
        void onLeave(const visitInfo& i, getExpr& me, nbool) override;
        void onLeave(const visitInfo& i, retExpr& me, nbool) override;
        void onLeave(const visitInfo& i, runExpr& me, nbool) override;
        void onTraverse(runExpr& me, node& subject) override;
        nbool onVisit(const visitInfo& i, func& me, nbool) override;
        void onLeave(const visitInfo& i, func& me, nbool) override;
        nbool onVisit(const visitInfo& i, closure& me, nbool) override;
        void onLeave(const visitInfo& i, closure& me, nbool) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool) override;
        void onLeave(const visitInfo& i, ctor& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool) override;
        void onLeave(const visitInfo& i, baseObj& me, nbool) override;
        nbool onVisit(const visitInfo& i, genericOrigin& me, nbool) override;
        void onLeave(const visitInfo& i, genericOrigin& me, nbool) override;
        nbool onVisit(const visitInfo& i, forExpr& me, nbool) override;
        void onLeave(const visitInfo& i, forExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, whileExpr& me, nbool) override;
        void onLeave(const visitInfo& i, breakExpr& me, nbool) override;
        void onLeave(const visitInfo& i, nextExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, ifExpr& me, nbool) override;
        void onLeave(const visitInfo& i, ifExpr& me, nbool) override;

    protected:
        void _prepare() override;
        void _onEndWork() override;

    private:
        std::string _asStr(const params& ps);

        // @param newInfo is not a heap instance.
        void _verifyMgdFuncImplicitReturn(func& me);
        void _onLeave(const visitInfo& i, const loopExpr& me);

    private:
        std::vector<loopExpr*> _recentLoops;
        std::vector<baseObj*> _orgs;
    };
} // namespace by
