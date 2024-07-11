#pragma once

#include "tstr.hpp"
#include "tweak.inl"
#include "strTactic.hpp"

namespace nm {

#define TEMPL   template <typename T, typename TACTIC>
#define ME  tstr<T, TACTIC>
#define SUPER   tweak<T, TACTIC>

    TEMPL ME::tstr(): SUPER() {}
    TEMPL ME::tstr(const type& subtype): SUPER(subtype) {}
    TEMPL ME::tstr(const T& it): SUPER() { this->bind(it); }
    TEMPL ME::tstr(const T* it): SUPER() { this->bind(*it); }
    TEMPL ME::tstr(const binder& rhs): SUPER() { this->bind(*rhs); }

#undef SUPER
#undef ME
#undef TEMPL

}
