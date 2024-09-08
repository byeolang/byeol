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
        void onLeave(const visitInfo& i, node& me) override;
        void onLeave(const visitInfo& i, asExpr& me) override;
        void onLeave(const visitInfo& i, isExpr& me) override;
        void onLeave(const visitInfo& i, assignExpr& me) override;
        nbool onVisit(const visitInfo& i, blockExpr& me) override;
        void onLeave(const visitInfo& i, blockExpr& me) override;
        void onLeave(const visitInfo& i, defVarExpr& me) override;
        void onLeave(const visitInfo& i, defPropExpr& me) override;
        void onLeave(const visitInfo& i, defAssignExpr& me) override;
        void onLeave(const visitInfo& i, defSeqExpr& me) override;
        void onLeave(const visitInfo& i, defArrayExpr& me) override;
        void onLeave(const visitInfo& i, FBOExpr& me) override;
        void onLeave(const visitInfo& i, FUOExpr& me) override;
        void onLeave(const visitInfo& i, getExpr& me) override;
        void onLeave(const visitInfo& i, retExpr& me) override;
        void onLeave(const visitInfo& i, runExpr& me) override;
        void onTraverse(runExpr& me, node& subject) override;
        nbool onVisit(const visitInfo& i, func& me) override;
        void onLeave(const visitInfo& i, func& me) override;
        nbool onVisit(const visitInfo& i, ctor& me) override;
        void onLeave(const visitInfo& i, ctor& me) override;
        nbool onVisit(const visitInfo& i, baseObj& me) override;
        void onLeave(const visitInfo& i, baseObj& me) override;
        nbool onVisit(const visitInfo& i, genericOrigin& me) override;
        void onLeave(const visitInfo& i, genericOrigin& me) override;
        nbool onVisit(const visitInfo& i, forExpr& me) override;
        void onLeave(const visitInfo& i, forExpr& me) override;
        nbool onVisit(const visitInfo& i, whileExpr& me) override;
        void onLeave(const visitInfo& i, breakExpr& me) override;
        void onLeave(const visitInfo& i, nextExpr& me) override;
        nbool onVisit(const visitInfo& i, ifExpr& me) override;
        void onLeave(const visitInfo& i, ifExpr& me) override;

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
