#pragma once

#include "visitor.hpp"
#include "../frame/frame.hpp"
#include "../ast/params.hpp"

struct verifierTest;

namespace namu {

    class loopExpr;
    class thread;
    class _nout verifier : public visitor {
        NAMU(CLASS(verifier, visitor))
        friend struct ::verifierTest;
        friend class genericObj;

    public:
        void onLeave(visitInfo i, node& me) override;
        void onLeave(visitInfo i, asExpr& me) override;
        void onLeave(visitInfo i, assignExpr& me) override;
        void onLeave(visitInfo i, blockExpr& me) override;
        void onLeave(visitInfo i, defVarExpr& me) override;
        void onLeave(visitInfo i, defAssignExpr& me) override;
        void onLeave(visitInfo i, defSeqExpr& me) override;
        void onLeave(visitInfo i, defArrayExpr& me) override;
        void onLeave(visitInfo i, FBOExpr& me) override;
        void onLeave(visitInfo i, FUOExpr& me) override;
        void onLeave(visitInfo i, getExpr& me) override;
        void onLeave(visitInfo i, retExpr& me) override;
        void onLeave(visitInfo i, runExpr& me) override;
        void onTraverse(runExpr& me, node& subject) override;
        nbool onVisit(visitInfo i, func& me) override;
        void onLeave(visitInfo i, func& me) override;
        nbool onVisit(visitInfo i, baseObj& me) override;
        void onLeave(visitInfo i, baseObj& me) override;
        nbool onVisit(visitInfo i, genericObj& me) override;
        void onLeave(visitInfo i, genericObj& me) override;
        nbool onVisit(visitInfo i, forExpr& me) override;
        void onLeave(visitInfo i, forExpr& me) override;
        nbool onVisit(visitInfo i, whileExpr& me) override;
        void onLeave(visitInfo i, whileExpr& me) override;
        void onLeave(visitInfo i, breakExpr& me) override;
        void onLeave(visitInfo i, nextExpr& me) override;
        nbool onVisit(visitInfo i, ifExpr& me) override;
        void onLeave(visitInfo i, ifExpr& me) override;
        void onTraverse(ifExpr& me, blockExpr& blk) override;

    protected:
        void _prepare() override;

    private:
        std::string _asStr(const params& ps);

        // @param newInfo is not a heap instance.
        void _verifyMgdFuncImplicitReturn(func& me);
        void _onLeave(const loopExpr& me);

    private:
        std::vector<loopExpr*> _recentLoops;
    };
}
