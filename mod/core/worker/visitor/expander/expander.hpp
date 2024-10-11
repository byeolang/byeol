#pragma once

#include "../visitor.hpp"
#include "convergence.hpp"

namespace nm {

    class _nout expander: public visitor {
        NM(CLASS(expander, visitor))

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

        nbool onVisit(const visitInfo& i, obj& me) override;
        void onLeave(const visitInfo& i, obj& me) override;
        nbool onVisit(const visitInfo& i, func& me) override;
        void onLeave(const visitInfo& i, func& me) override;
        nbool onVisit(const visitInfo& i, baseFunc& me) override;
        void onLeave(const visitInfo& i, baseFunc& me) override;
        nbool onVisit(const visitInfo& i, getGenericExpr& me) override;

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
        tnarr<convergence> _cons;
        tstr<obj> _obj;
        tstr<func> _func;
    };
} // namespace nm
