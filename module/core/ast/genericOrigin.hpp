/// @file
#pragma once

#include "core/ast/origin.hpp"

namespace by {

    class visitor;
    class verifier;

    /// @ingroup core
    /// @brief Generic type origin with type parameters
    /// @details Manages generic type definitions with type parameter substitution.
    /// Caches instantiated generic types and handles type parameter binding.
    class _nout genericOrigin: public baseObj {
        BY(CLASS(genericOrigin, baseObj), VISIT())
        friend class verifier;
        friend class graphVisitor;
        friend class exprMaker;
        typedef std::vector<std::string> strings;

    public:
        genericOrigin(const origin& org, const strings& paramNames);

    public:
        const baseObj& getOrigin() const override;
        const strings& getParamNames() const;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        str run(const args& a) override;

        str infer() const override;

        scope& subs() override;

        const std::map<std::string, tstr<obj>>& getCache() const;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;

        using super::outFrame;
        void outFrame() const override;

    protected:
        str _onRunSub(node& sub, const args& a) override;

    private:
        std::string _makeKey(const args& a) const;

        /// make a generic object.
        tstr<obj> _makeGeneric(const std::string& argName, const params& ps);
        nbool _isSelfMaking(const std::string& key) const;

    private:
        std::map<std::string, tstr<obj>> _cache;
        tstr<obj> _org;
        strings _paramNames;
    };
} // namespace by
