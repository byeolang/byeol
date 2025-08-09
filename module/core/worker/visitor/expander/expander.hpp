#pragma once

#include "core/worker/visitor/visitor.hpp"
#include "core/worker/visitor/expander/convergence/baseConvergence.hpp"

namespace by {

    class _nout expander: public visitor {
        BY(CLASS(expander, visitor))

        struct _nout expansion {
            nbool isExpanded() const;

        public:
            tweak<obj> me;
            tweak<func> fun;
        };

    public:
        expander();

    public:
        void rel() override;

        using super::onVisit;

        nbool onVisit(const visitInfo& i, defAssignExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, asExpr& me, nbool) override;
        nbool onVisit(const visitInfo& i, obj& me, nbool) override;
        void onLeave(const visitInfo& i, obj& me, nbool) override;
        nbool onVisit(const visitInfo& i, func& me, nbool) override;
        void onLeave(const visitInfo& i, func& me, nbool) override;
        nbool onVisit(const visitInfo& i, baseFunc& me, nbool) override;
        void onLeave(const visitInfo& i, baseFunc& me, nbool) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me, nbool) override;

    protected:
        void _onWork() override;

    private:
        nbool _onVisitParams(baseFunc& f, param& p);
        void _onVisitFuncRet(baseFunc& f);

        void _convergeTypes(errReport& rpt);

        void _rel();

        /// @return true if there is a change.
        nbool _expandAll(errReport& rpt);
        /// @return true if there is a change.
        nbool _expand(errReport& rpt, expansion& eval);
        void _delEval(const std::map<obj*, expansion>::iterator& e);
        void _expand();
        nbool _isAllExpanded(obj& key) const;

    private:
        std::map<obj*, expansion> _stack;
        tnarr<baseConvergence> _cons;
        std::vector<obj*> _obj;
        std::vector<baseFunc*> _funcs;
    };
} // namespace by
