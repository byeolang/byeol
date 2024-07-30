#pragma once

#include "binder.hpp"
#include "weakTactic.hpp"

namespace nm {

    class origin;

    template <typename T, typename TACTIC = weakTactic> class tweak: public binder {
        NM_ME(tweak, binder)
        NM_INIT_META(me)
        friend class bindTag;

    public:
        /// tweak:
        tweak();
        tweak(const type& subtype);
        tweak(const T& it);
        tweak(const T* it);
        tweak(const me& rhs);
        explicit tweak(const binder& rhs);

    public:
        T* operator->();
        T& operator*();
        const T* operator->() const NM_CONST_FUNC(operator->())
        const T& operator*() const NM_CONST_FUNC(operator*())
        me& operator=(const binder& rhs);
        me& operator=(const me& rhs);

    public:
        using super::get;
        T& get();
        const T& get() const NM_CONST_FUNC(get())

        using tbindable::bind;
        nbool bind(const T& new1);
    };
} // namespace nm
