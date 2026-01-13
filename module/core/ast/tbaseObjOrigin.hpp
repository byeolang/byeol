/// @file
#pragma once

#include "core/ast/baseObj.hpp"

namespace by {

    /** @ingroup core
     *  @brief Template for easily defining baseObj origin objects
     *  @details A class template used to easily define @ref origin objects for baseObj.
     *
     *  @remark See origin and @ref baseObj first.
     *
     *  The structure and concept are identical to the origin class. It shadows its own type and is intended to be
     *  treated as baseObj type. However, differences arise in usage. This is because the baseObj class concept differs
     *  from obj.
     *
     *  @remark Unlike obj, baseObj's origin object exists only once. See baseObj for details.
     *
     *  @warning This limitation affects binder usage too. Simply declaring a binder with type parameter
     *  'baseObjOrigin' is not allowed. Use 'baseObj' type instead.
     *
     *  @code
     *      tstr<baseObjOrigin> a; // X, unexpected behavior may happen.
     *      tstr<baseObj> a;       // O
     *  @endcode
     */
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
