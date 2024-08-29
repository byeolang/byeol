#pragma once

#include "mockNode.hpp"
#include "baseObj.hpp"

namespace nm {

    /// this baseObjOrigin class is represents user defined classes in unmanaged codes.
    /// one important thing you must remember is, origin should be shadowed to 'baseObj' type.
    /// every interaction you can take with origin could be handled with baseObj class.
    /// and inherits something too origin class aren't allowed.
    ///
    /// this limitation affects to usage of binder too:
    /// simply, declaring binder with type parameter 'baseObjOrigin' is not allowed. use 'baseObj'
    /// type instead of.
    ///     e.g.
    ///         tstr<baseObjOrigin> a; // X, unexpected behavior may happen.
    ///         tstr<baseObj> a; // O
    template <typename T> class tbaseObjOrigin: public T {
        NM(ME(tbaseObjOrigin, T), INIT_META(tbaseObjOrigin))
        static_assert(tifSub<T, baseObj>::is, "you need to input 'T' as derived class of baseObj.");

    public:
        tbaseObjOrigin() { this->_setOrigin(*this); }

        tbaseObjOrigin(const src& s, const scope& subs): me(s, subs, *new modifier(true, false)) {}

        tbaseObjOrigin(const src& s, const scope& subs, const modifier& mod):
            super(), _src(s), _subs(subs), _mod(mod) {
            this->_setOrigin(*this);
        }

    public:
        const node& getSubPack() const override {
            static mockNode inner;
            return inner;
        }

        const ntype& getType() const override { return ttype<super>::get(); }

        const src& getSrc() const override { return *_src; }

        using super::subs;

        scope& subs() override { return *_subs; }

        const modifier& getModifier() const override { return *_mod; }

        clonable* clone() const override { return new super(*this); }

        clonable* cloneDeep() const override {
            // TODO: deelcopy member variables.
            return new me(*this);
        }

    private:
        tstr<src> _src;
        tstr<scope> _subs;
        tstr<modifier> _mod;
    };
} // namespace nm
