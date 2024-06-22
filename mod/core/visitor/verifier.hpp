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
        nbool onVisit(visitInfo i, node& me) override;
        nbool onVisit(visitInfo i, asExpr& me) override;
        nbool onVisit(visitInfo i, assignExpr& me) override;
        nbool onVisit(visitInfo i, blockExpr& me) override;
        void onLeave(visitInfo i, blockExpr& me) override;
        void onLeave(visitInfo i, defAssignExpr& me) override;
        nbool onVisit(visitInfo i, defPropExpr& me) override;
        nbool onVisit(visitInfo i, defSeqExpr& me) override;
        nbool onVisit(visitInfo i, defArrayExpr& me) override;
        nbool onVisit(visitInfo i, FBOExpr& me) override;
        nbool onVisit(visitInfo i, FUOExpr& me) override;
        nbool onVisit(visitInfo i, getExpr& me) override;
        nbool onVisit(visitInfo i, retExpr& me) override;
        nbool onVisit(visitInfo i, runExpr& me) override;
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
        nbool onVisit(visitInfo i, breakExpr& me) override;
        nbool onVisit(visitInfo i, nextExpr& me) override;
        nbool onVisit(visitInfo i, ifExpr& me) override;
        void onLeave(visitInfo i, ifExpr& me) override;

    protected:
        void _prepare() override;

    private:
        std::string _asStr(const params& ps);

        // @param newInfo is not a heap instance.
        void _verifyMgdFuncImplicitReturn(func& me);
        nbool _isVariableDuplicated(defAssignExpr& me, const node& subs);
        nbool _verifyArrWhetherHaveRetExpr(const arr& a, const expr& me);

    private:
        std::vector<loopExpr*> _recentLoops;
    };
}
