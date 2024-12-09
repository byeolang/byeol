#pragma once

#include "../../ast/params.hpp"
#include "../../frame/frame.hpp"
#include "visitor.hpp"

struct verifierTest;

namespace nm {

    class loopExpr;
    class thread;
    class endExpr;

    class _nout verifier: public visitor {
        NM(CLASS(verifier, visitor))
        friend struct ::verifierTest;
        friend class genericOrigin;

    public:
        void onLeave(const visitInfo& i, node& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, asExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, isExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, assignExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, blockExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, blockExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defVarExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defPropExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defAssignExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defSeqExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defArrayExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, defNestedFuncExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, FBOExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, FUOExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, getExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, retExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, runExpr& me, nbool alreadyVisited) override;
        void onTraverse(runExpr& me, node& subject) override;
        nbool onVisit(const visitInfo& i, func& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, func& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, closure& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, ctor& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, ctor& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, baseObj& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, baseObj& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, obj& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, genericOrigin& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, genericOrigin& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, forExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, forExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, whileExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, breakExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, nextExpr& me, nbool alreadyVisited) override;
        nbool onVisit(const visitInfo& i, ifExpr& me, nbool alreadyVisited) override;
        void onLeave(const visitInfo& i, ifExpr& me, nbool alreadyVisited) override;

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
} // namespace nm
