#pragma once

#include "meta/common.hpp"
#include "meta/type/ttype.hpp"

namespace nm {
    class _nout typeProvidable {
        NM_ME(typeProvidable)

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual const type& getType() const = 0;

        nbool isSub(const type& it) const;
        nbool isSub(const type* it) const NM_SIDE_FUNC(isSub);
        nbool isSub(const me& it) const;
        nbool isSub(const me* it) const NM_SIDE_FUNC(isSub);

        nbool isSuper(const type& it) const;
        nbool isSuper(const type* it) const NM_SIDE_FUNC(isSuper);
        nbool isSuper(const me& it) const;

        nbool isSuper(const me* it) const NM_SIDE_FUNC(isSuper);

        template <typename T> nint isSub() const {
            return getType().isSub<T>();
        }

        template <typename T> nint isSuper() const { return getType().isSuper<T>(); }

        template <typename T> T* cast() { return (T*) cast(ttype<T>::get()); }

        template <typename T> const T* cast() const NM_CONST_FUNC(cast<T>())

        virtual void* cast(const type& to);
        const void* cast(const type& to) const NM_CONST_FUNC(cast(to))
        void* cast(const type* it) NM_SIDE_FUNC(cast);
        const void* cast(const type* to) const NM_CONST_FUNC(cast(to))

    protected:
        virtual nbool _onSame(const me& rhs) const;
    };
} // namespace nm
