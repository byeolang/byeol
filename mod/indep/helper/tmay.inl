#pragma once

#include "tmay.hpp"
#include "nulr.hpp"

namespace nm {

#define ME tmay<T>
#define TEMPLATE template <typename T>

    TEMPLATE
    ME::tmay(const T& value): _subj(value) {}

    TEMPLATE
    T* ME::operator->() { return &get(); }

    TEMPLATE
    T& ME::operator*() { return get(); }

    TEMPLATE
    ME::operator nbool() const { return has(); }

    TEMPLATE
    nbool ME::has() const { return _subj.has_value(); }

    TEMPLATE
    T& ME::get() {
        if(!has()) return nulOf<T>();
        return _subj.value();
    }

    TEMPLATE
    void ME::rel() { _subj.reset(); }

    TEMPLATE
    template <typename... Ts> void ME::set(Ts... args) {
        _subj.emplace(args...);
    }

#undef TEMPLATE
#undef ME
} // namespace nm
