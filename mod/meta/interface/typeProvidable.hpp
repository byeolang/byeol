#pragma once

#include "../common.hpp"
#include "type/ttype.hpp"

namespace nm {
    class _nout typeProvidable {
        NM_ME(typeProvidable)

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual const type& getType() const = 0;
        nbool isSub(const type& it) const;
        nbool isSub(const me& it) const;
        nbool isSuper(const type& it) const;
        nbool isSuper(const me& it) const;

        template <typename T> nint isSub() const { return getType().isSub<T>(); }

        template <typename T> nint isSuper() const { return getType().isSuper<T>(); }

        template <typename T> T& cast() {
            // this protection:
            //  this c namu interpreter uses references in default, and not any pointers.
            //  so you always care about a refer can be nulled.
            //  for a convenience of our API users I put this guard.
            //
            //  please note that checking whether thisptr is null isn't recommended in
            //  ordinary cpp.
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-undefined-compare"
            if(this == nullptr) return nulOf<T>();
#pragma clang diagnostic pop

            return *(T*) cast(ttype<T>::get());
        }

        /// namu is using a null reference.
        /// you should always try to cast null references to other types only via cast<T> or
        /// safeCast<T> instead of (T&) because attempting to cast direclty a null reference to
        /// another type can result in a garbage address.
        template <typename T, typename A> static T& safeCast(const A& any) {
            return nul(any) ? nulOf<T>() : (T&) any;
        }

        template <typename T> const T& cast() const NM_CONST_FUNC(cast<T>())

        virtual void* cast(const type& to);
        const void* cast(const type& to) const NM_CONST_FUNC(cast(to))

    protected:
        virtual nbool _onSame(const me& rhs) const;
    };
} // namespace nm
