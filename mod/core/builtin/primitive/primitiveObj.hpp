#pragma once

#include "arithmeticObj.hpp"
#include "../../ast/func.hpp"
#include "../../ast/scope.hpp"
#include "../../ast/dumScope.hpp"
#include "../../ast/src/dumSrc.hpp"

namespace nm {

    template <typename T>
    class primitiveObj : public arithmeticObj {
        NM(ADT(primitiveObj, arithmeticObj))
        typedef T trait;

    protected:
        template <typename E, typename RAW>
        class asPrimitive : public tas<E> {
            str as(const node& me, const type& to) const override {
                return str(new E(me.cast<RAW>()));
            }
        };

    public:
        primitiveObj(): _val() {}
        primitiveObj(const T& val): _val(val) {}

    public:
        T& get() {
            return _val;
        }
        const T& get() const {
            return _val;
        }

        using super::cast;
        void* cast(const type& to) override {
            if(to == ttype<T>::get())
                return &_val;

            return super::cast(to);
        }

        using super::subs;
        scope& subs() override {
            static tstr<scope> inner;
            if(!inner) {
                scope* scapegoat = new scope();
                _onMakeSubs(*scapegoat);
                inner.bind(new dumScope(*scapegoat));
            }

            return *inner;
        }

    protected:
        nbool _onSame(const typeProvidable& rhs) const override {
            const me& cast = (const me&) rhs;
            return _val == cast._val;
        }

        virtual void _onMakeSubs(scope& tray) const = 0;

    private:
        T _val;
    };

    template <>
    class primitiveObj<void> : public arithmeticObj {
        NM(ADT(primitiveObj, arithmeticObj))

    public:
        const super& getOrigin() const override {
            return *this;
        }

        using super::subs;
        scope& subs() override {
            static tstr<scope> inner;
            if(!inner) {
                scope* scapegoat = new scope();
                _onMakeSubs(*scapegoat);
                inner.bind(new dumScope(*scapegoat));
            }

            return *inner;
        }

    protected:
        nbool _onSame(const typeProvidable& rhs) const override {
            return !nul(rhs);
        }

        virtual void _onMakeSubs(scope& tray) const = 0;
    };
}
