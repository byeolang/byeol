/// @file
#pragma once

#include "core/ast/origin.hpp"

namespace by {

    class visitor;
    class verifier;

    /** @ingroup core
     *  @brief Generic type origin with type parameters
     *  @details Basically performs the same role as origin class, but differs in that it supports generics and that
     *  this instance itself is not responsible for origin but creates/manages origin as needed.
     *
     *  @section operation_method Operation Method
     *  genericOrigin manages origin objects with a map. When there's no `myObj` key in it, it deepClones the original
     *  origin object _org to create a new @ref origin object. Then it puts generalizer into that object to perform the
     *  operation of replacing all type parameter T with myObj, changing the existing getExpr(args = T) parts to
     *  getExpr(args = myObj). The replaced origin is registered in subs() so that when called later with the same key,
     *  it's returned immediately without generalizer.
     */
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

        str eval(const args& a) override;

        str infer() const override;

        scope& subs() override;

        const std::map<std::string, tstr<obj>>& getCache() const;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;

        using super::outFrame;
        void outFrame() const override;

    protected:
        str _onEvalSub(node& sub, const args& a) override;

    private:
        std::string _makeKey(const args& a) const;

        /**
         *  make a generic object.
         */
        tstr<obj> _makeGeneric(const std::string& argName, const params& ps);
        nbool _isSelfMaking(const std::string& key) const;

    private:
        std::map<std::string, tstr<obj>> _cache;
        tstr<obj> _org;
        strings _paramNames;
    };
} // namespace by
