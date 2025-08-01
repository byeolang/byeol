/// @file
#pragma once

#include "core/ast/tmock.hpp"
#include "core/ast/baseObj.hpp"

namespace by {

    /// @ingroup core
    /// @brief Template base object @ref origin for user-defined classes
    /// @details Template for creating origin objects for user-defined classes in unmanaged code.
    /// @note Origin should be shadowed to 'baseObj' type for proper interaction.
    /// this baseObjOrigin class is represents user defined classes in unmanaged codes.
    /// one important thing you must remember is, origin should be shadowed to 'baseObj' type.
    /// every interaction you can take with origin could be handled with baseObj class.
    /// and inherits something too origin class aren't allowed.
    ///
    /// @warning this limitation affects to usage of binder too.
    /// simply, declaring binder with type parameter 'baseObjOrigin' is not allowed. use 'baseObj'
    /// type instead of.
    /// @code
    ///     tstr<baseObjOrigin> a; // X, unexpected behavior may happen.
    ///     tstr<baseObj> a; // O
    /// @endcode
    template <typename T> class tbaseObjOrigin: public T {
        BY(ME(tbaseObjOrigin, T), INIT_META(tbaseObjOrigin))
        static_assert(tifSub<T, baseObj>::is, "you need to input 'T' as derived class of baseObj.");

    public:
        tbaseObjOrigin(): me(dumScope::singleton()) {}

        tbaseObjOrigin(const scope& subs): me(subs, *new modifier()) {}

        tbaseObjOrigin(const scope& subs, const modifier& mod): super(), _subs(subs), _mod(mod) {
            this->_setOrigin(*this);
        }

    public:
        const node& getSubPack() const override {
            static mockNode inner;
            return inner;
        }

        const ntype& getType() const override { return ttype<super>::get(); }

        using super::subs;

        scope& subs() override { return *_subs; }

        const modifier& getModifier() const override { return *_mod; }

        clonable* clone() const override {
            baseObj* ret = new super(*this);
            ret->_setOrigin(*this);
            return ret;
        }

        clonable* cloneDeep() const override {
            me* ret = new me(*this);
            ret->onCloneDeep(*this);
            return ret;
        }

        void onCloneDeep(const clonable& from) override {
            const me& rhs = (const me&) from;
            if(rhs._subs) _subs.bind((scope&) *rhs._subs->cloneDeep());
            if(rhs._mod) _mod.bind((modifier&) *rhs._mod->cloneDeep());
        }

    protected:
        void _setModifier(const modifier& mod) override { _mod.bind(mod); }

    private:
        tstr<scope> _subs;
        tstr<modifier> _mod;
    };
} // namespace by
