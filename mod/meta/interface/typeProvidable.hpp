#pragma once

#include "../common.hpp"

namespace nm {
    class _nout typeProvidable {
        NM_ME(typeProvidable)

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual const type& getType() const = 0;
        nbool isSub(const type& it) const;
        nbool isSub(const typeProvidable& it) const;
        nbool isSuper(const type& it) const;
        template <typename T> nbool isSub() const { return getType().isSub<T>(); }
        template <typename T> nbool isSuper() const { return getType().isSuper<T>(); }

        template <typename T>
        T& cast() {
            // this protection:
            //  this c namu interpreter uses references in default, and not any pointers.
            //  so you always care about a refer can be nulled.
            //  for a convenience of our API users I put this guard.
            //
            //  please note that checking whether thisptr is null isn't recommended in
            //  ordinary cpp.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
            if (this == nullptr)
                return nulOf<T>();
#pragma clang diagnostic pop

            return *(T*) cast(ttype<T>::get());
        }

        template <typename T> const T& cast() const NM_CONST_FUNC(cast<T>())

        virtual void* cast(const type& to);
        const void* cast(const type& to) const NM_CONST_FUNC(cast(to))

    protected:
        virtual nbool _onSame(const me& rhs) const;
    };
}
