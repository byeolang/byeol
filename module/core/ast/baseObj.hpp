/// @file
#pragma once

#include "core/frame/frameInteractable.hpp"
#include "core/ast/src/src.hpp"
#include "core/ast/statable.hpp"
#include "core/ast/modifier/modifier.hpp"
#include "core/ast/tmock.hpp"

namespace by {

    class baseFunc;
    typedef tnarr<baseFunc> funcs;
    class frame;
    class origin;
    class obj;
    class mgdType;
    template <typename T> class tbaseObjOrigin;

    /// @ingroup core
    /// @brief Base class for all @ref obj "objects" in byeol language
    /// @details Handles @ref frame injection events and provides common @ref obj "object" functionality.
    /// Manages object @ref origin, state, @ref modifier "modifiers", and @ref frame interactions for all byeol objects.
    class _nout baseObj: public node, public statable {
        BY(ADT(baseObj, node))
        friend class verifier;
        friend class obj;
        friend class defaultMakeCtor;
        friend class parser;        // _setOrigin()
        friend class genericOrigin; // from genericOrigin::_makeGeneric(), _setOrigin()
        friend class exprMaker;
        friend class defVarExpr; // for _setModifier
        template <typename T> friend class tbaseObjOrigin;

    protected:
        /// if you don't give any subs when construct an @ref baseObj, _subs will be assigned to dummy
        /// array. instance on ctor of derived class.
        explicit baseObj() = default;
        explicit baseObj(const baseObj* org, nbool);

    public:
        using super::eval;
        str eval(const args& a) override;

        using super::subs;
        scope& subs() override;

        state getState() const override;
        void setState(state new1) override;

        using super::prioritize;
        priorType prioritize(const args& a) const override;

        virtual const baseObj& getOrigin() const;

        using super::inFrame;
        void inFrame(const bicontainable* args) const override;
        using super::outFrame;
        void outFrame() const override;

        const src& getSrc() const override;

        virtual const node& getSubPack() const;

        virtual baseObj* make() const;

        const modifier& getModifier() const override;

    protected:
        str _onRunSub(node& sub, const args& a) override;
        void _setSrc(const src& s) override;
        virtual void _inFrame(frame& fr, const bicontainable* args) const;

        // update @ref origin pointer of an object.
        // to modify origin* is very dangerous. only permitted module should do this.
        void _setOrigin(const baseObj& newOrg);

        virtual void _setType(const mgdType& new1);

        virtual void _setModifier(const modifier& mod);

        virtual str _onBeforeCtor();

    public:
        inline static const std::string COMMON_NAME = "@common";
        inline static const std::string EXPAND_NAME = "@expand";

    private:
        tstr<baseObj> _org;
    };

    typedef tmock<baseObj> mockObj;
} // namespace by
