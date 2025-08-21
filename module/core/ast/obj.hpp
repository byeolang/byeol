/// @file
#pragma once

#include "core/ast/baseObj.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/statable.hpp"

namespace by {

    class mgdType;
    class origin;
    class runExpr;

    /// @ingroup core
    /// @brief Managed object in byeol programming environment
    /// @details Represents objects structured in the managed byeol environment.
    /// Owned sub nodes can only be manipulated through the obj API, unlike
    /// native objects which only have shared nodes.
    class _nout obj: public baseObj {
        BY(ME(obj, baseObj), INIT_META(obj), CLONE(obj), VISIT())

    public:
        typedef ntype metaType;
        friend class slot;

    public:
        explicit obj(); // this means 'any' class.
        explicit obj(const scope& subs);
        explicit obj(const me& rhs);

    protected:
        explicit obj(const scope& shares, scope& owns);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        tstr<nbicontainer> mySubs() const override;

        state getState() const override;
        void setState(state new1) override;

        scope& getShares();
        const scope& getShares() const BY_CONST_FUNC(getShares())
        scope::super& getOwns();
        const scope::super& getOwns() const BY_CONST_FUNC(getOwns())

        const ntype& getType() const override;

        void onCloneDeep(const clonable& from) override;

        virtual node* getCallComplete();
        const node* getCallComplete() const BY_CONST_FUNC(getCallComplete())

    protected:
        void _inFrame(frame& fr, const bicontainable* args) const override;
        str _onBeforeCtor() override;

    private:
        me& _assign(const me& rhs);

    private:
        tstr<scope> _subs;
    };

#ifdef BY_BUILD_PLATFORM_IS_WINDOWS
    // f***ing annoying another MSVC bug here:
    //  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
    //  I used plenty hours of heading to the ground.
    //
    //  I don't know why, but unless define below variable here, I'll observe that the
    //  member-variable
    //  '_subs' above was tried to be instantiated but failed.
    //  error message said that 'You've used undefined type "identifiertstr<scope>"'.
    //  however, MSVC definately knows about tstr<T> and scope types.
    //
    //  clang, gcc didn't complain about this.
    namespace {
        static const inline scope a3;
    }
#endif
} // namespace by
