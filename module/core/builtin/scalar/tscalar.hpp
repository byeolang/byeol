/// @file
#pragma once

#include "core/ast/dumScope.hpp"
#include "core/ast/func.hpp"
#include "core/ast/scope.hpp"
#include "core/ast/src/dumSrc.hpp"
#include "core/type/as/tas.hpp"
#include "core/builtin/scalar/scalar.hpp"

namespace by {

    template <typename T> class tscalar: public scalar {
        BY(ADT(tscalar, scalar))
        typedef T trait;

    protected:
        template <typename E, typename RAW> class asPrimitive: public tas<E, T> {
            typedef tas<E, T> __super18;
            BY(CLASS(asPrimitive, __super18))

        public:
            str as(const node& me, const type& to) const override { return str(new E(*me.cast<RAW>())); }

        protected:
            str _onAs(const T& me, const type& to) const override { return str(); }
        };

    public:
        tscalar(): _val() {}

        tscalar(const T& val): _val(val) {}

    public:
        T& get() { return _val; }

        const T& get() const { return _val; }

        using super::cast;

        void* cast(const type& to) override {
            WHEN(to == ttype<T>::get()) .ret(&_val);

            return super::cast(to);
        }

    protected:
        nbool _onSame(const typeProvidable& rhs) const override {
            const me& cast = (const me&) rhs;
            return _val == cast._val;
        }

    private:
        T _val;
    };

    template <> class tscalar<void>: public scalar {
        BY(ADT(tscalar, scalar))

    protected:
        nbool _onSame(const typeProvidable& rhs) const override { return true; }
    };
} // namespace by
