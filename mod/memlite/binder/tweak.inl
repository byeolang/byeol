#pragma once

#include "memlite/binder/tbindable.inl"
#include "memlite/binder/tweak.hpp"
#include "memlite/binder/weakTactic.hpp"

namespace nm {

#define TEMPL template <typename T, typename TACTIC>
#define ME tweak<T, TACTIC>
#define SUPER binder

    TEMPL ME::tweak(): SUPER(ttype<T>::get(), TACTIC::singleton) {}

    TEMPL ME::tweak(const type& subtype):
        SUPER(subtype.isSub(ttype<T>::get()) ? subtype : ttype<T>::get(), TACTIC::singleton) {}

    TEMPL ME::tweak(const T& it): SUPER(ttype<T>::get(), TACTIC::singleton) { this->bind(it); }

    TEMPL ME::tweak(const T* it): SUPER(ttype<T>::get(), TACTIC::singleton) { this->bind(*it); }

    TEMPL ME::tweak(const ME& rhs): SUPER(ttype<T>::get(), TACTIC::singleton) {
        this->_assign(rhs);
    }

    TEMPL ME::tweak(const binder& rhs): SUPER(ttype<T>::get(), TACTIC::singleton) {
        this->bind(*rhs);
    }

    TEMPL T* ME::operator->() { return &this->get(); }

    TEMPL T& ME::operator*() { return *get(); }

    TEMPL ME& ME::operator=(const binder& rhs) {
        if(this == &rhs) return *this;

        SUPER::operator=(rhs);
        return *this;
    }

    TEMPL ME& ME::operator=(const me& rhs) { return operator=((const binder&) rhs); }

    TEMPL T* ME::get() {
        instance* got = SUPER::get() OR.retNul<T>();
        return got->cast<T>();
    }

    TEMPL nbool ME::bind(const T& new1) { return SUPER::bind(new1); }

#undef SUPER
#undef ME
#undef TEMPL

} // namespace nm
