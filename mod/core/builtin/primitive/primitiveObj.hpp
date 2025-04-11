#pragma once

#include "../../ast/dumScope.hpp"
#include "../../ast/func.hpp"
#include "../../ast/scope.hpp"
#include "../../ast/src/dumSrc.hpp"
#include "../../type/as/tas.hpp"
#include "arithmeticObj.hpp"

namespace nm {

    template <typename T> class primitiveObj: public arithmeticObj {
        NM(ADT(primitiveObj, arithmeticObj))
        typedef T trait;

    protected:
        template <typename E, typename RAW> class asPrimitive: public tas<E> {
            NM(CLASS(asPrimitive, tas<E>))

        public:
            str as(const node& me, const type& to) const override {
                return str(new E(me.cast<RAW>()));
            }
        };

    public:
        primitiveObj() = default;

        primitiveObj(const T& val): _val(val) {}

    public:
        T& get() { return _val; }

        const T& get() const { return _val; }

        using super::cast;

        void* cast(const type& to) override {
            WHEN(to == ttype<T>::get()).ret(&_val);

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

    template <> class primitiveObj<void>: public arithmeticObj {
        NM(ADT(primitiveObj, arithmeticObj))

    protected:
        nbool _onSame(const typeProvidable& rhs) const override { return !nul(rhs); }
    };
} // namespace nm
