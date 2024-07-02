#pragma once

#include "visitor.hpp"

namespace nm {

    class _nout preEvaluator : public visitor {
        NAMU(CLASS(preEvaluator, visitor))

        struct _nout evaluation {
            nbool isEvaluated() const;

        public:
            tweak<obj> me;
            tweak<func> fun;
        };

    public:
        preEvaluator();

    public:
        void rel() override;

        using super::onVisit;

        nbool onVisit(visitInfo i, obj& me) override;
        void onLeave(visitInfo i, obj& me) override;
        nbool onVisit(visitInfo i, func& me) override;
        void onLeave(visitInfo i, func& me) override;
        nbool onVisit(visitInfo i, getGenericExpr& me) override;

    protected:
        void _onWork() override;

    private:
        void _rel();

        /// @return true if there is a change.
        nbool _tryPreEvals(errReport& rpt);
        /// @return true if there is a change.
        nbool _tryPreEval(errReport& rpt, evaluation& eval);
        void _delEval(std::map<obj*, evaluation>::iterator e);
        void _preEval();
        nbool _isAllEvaluated(obj& key) const;

    private:
        std::map<obj*, evaluation> _stack;
        tstr<obj> _obj;
        tstr<func> _func;
    };
}
