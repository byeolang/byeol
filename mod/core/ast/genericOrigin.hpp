#pragma once

#include "origin.hpp"

namespace nm {

    class visitor;
    class verifier;

    class _nout genericOrigin : public baseObj {
        NM(CLASS(genericOrigin, baseObj), VISIT())
        friend class verifier;
        friend class graphVisitor;
        friend class exprMaker;
        typedef std::vector<std::string> strings;

    public:
        genericOrigin(const origin& org, const strings& paramNames);

    public:
        const baseObj& getOrigin() const override;
        const strings& getParamNames() const;

        priorType prioritize(const args& a) const override;
        str run(const args& a) override;

        str getEval() const override;

        scope& subs() override;

        const std::map<std::string, tstr<obj>>& getCache() const;

        using super::inFrame;
        void inFrame(const bicontainable& args) override;
        using super::outFrame;
        void outFrame(const bicontainable& args) override;

    protected:
        str _onRunSub(node& sub, const args& a) override;

    private:
        std::string _makeKey(const args& a) const;

        /// make a generic object.
        tstr<origin> _makeGeneric(const std::string& argName, const args& a) const;

    private:
        std::map<std::string, tstr<obj>> _cache;
        tstr<origin> _org;
        strings _paramNames;
    };
}
