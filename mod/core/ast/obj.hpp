#pragma once

#include "baseObj.hpp"
#include "scope.hpp"

namespace namu {

    class mgdType;

    /// obj is a object structured in managed programming environment like 'namu'.
    /// owned sub nodes of a object can only be manipulated through API provided obj.
    /// because native object won't have owned nodes but only shared ones.
    class _nout obj : public baseObj {
        NAMU(ME(obj, baseObj),
             INIT_META(obj),
             CLONE(obj),
             VISIT())
        friend class slot; // for _onRunSub
        friend class parser;
        friend class preEvaluator;
        friend class genericObj; // for _setOrigin(), _setType
        friend class exprMaker;

    public:
        explicit obj();
        explicit obj(scope& shares, scope& owns);
        explicit obj(mgdType* newType);
        explicit obj(mgdType* newType, scope& shares, scope& owns);
        explicit obj(const me& rhs);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        tstr<nbicontainer> mySubs() const override;

        using super::run;
        str run(const args& a) override;

        scope& getShares();
        const scope& getShares() const NAMU_UNCONST_FUNC(getShares())
        scope::super& getOwns();
        const scope::super& getOwns() const NAMU_UNCONST_FUNC(getOwns())
        const obj& getOrigin() const override;
        const obj& getSubPack() const;

        clonable* cloneDeep() const override;
        typedef ntype metaType;
        const ntype& getType() const override;
        nbool isComplete() const override;

        nbool isPreEvaluated() const override;

    protected:
        void _inFrame(frame& fr, const bicontainable& args) override;

    private:
        void _setType(const mgdType* new1);
        // update origin pointer of an object.
        // to modify origin* is very dangerous. only permitted module should do this.
        void _setOrigin(obj* newOrg);
        void _setSubPack(const obj& newSub);

        me& _assign(const me& rhs);

        void _setComplete(nbool isComplete);

    private:
        tstr<scope> _subs;
        tstr<obj> _subpack; // TODO: this should be beloned to originObj class only.
        obj* _org; // TODO: this should be 'originObj*' type.
        point _pos;
        const mgdType* _type; // TODO: memory leak
        nbool _isComplete;
    };

#ifdef NAMU_BUILD_PLATFORM_IS_WINDOWS
    // f***ing annoying another MSVC bug here:
    //  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
    //  I used plenty hours of heading to the ground.
    //
    //  I don't know why, but unless define below variable here, I'll observe that the member-variable
    //  '_subs' above was tried to be instantiated but failed.
    //  error message said that 'You've used undefined type "identifiertstr<scope>"'.
    //  however, MSVC definately knows about tstr<T> and scope types.
    //
    //  clang, gcc didn't complain about this.
    namespace {
        static const inline scope a3;
    }
#endif
}
