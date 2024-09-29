#pragma once

#include "visitor.hpp"

namespace nm {

    class func;
    class param;

    struct typeConvergence {
        typeConvergence(param* p, node* org): _p(p), _org(org) {}

    public:
        void converge() {
            if(!_p || !_org) return;
            _p->setOrigin(*_org);
            NM_DI("typeConverge: %s", *_org);
        }

    private:
        param* _p;
        node* _org;
    };

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
        nbool onVisit(const visitInfo& i, getGenericExpr& me) override;

    protected:
        void _onWork() override;

    private:
        void _onVisitParamIfGetExpr(func& f, param& p);

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
        std::vector<typeConvergence> _cons;
        tstr<obj> _obj;
        tstr<func> _func;
    };
} // namespace nm
