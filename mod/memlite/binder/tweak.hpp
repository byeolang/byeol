#pragma once

#include "binder.hpp"
#include "weakTactic.hpp"

namespace nm {

    template <typename T, typename TACTIC = weakTactic>
    class tweak : public binder {
        NAMU_ME(tweak, binder)
        NAMU_INIT_META(me)
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
        const T* operator->() const NAMU_UNCONST_FUNC(operator->())
        const T& operator*() const NAMU_UNCONST_FUNC(operator*())
        me& operator=(const binder& rhs);

    public:
        using super::get;
        T& get();
        const T& get() const NAMU_UNCONST_FUNC(get())

        using tbindable::bind;
        nbool bind(const T& new1);
    };
}
