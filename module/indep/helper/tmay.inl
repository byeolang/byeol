#pragma once

#include "indep/helper/tmay.hpp"
#include "indep/helper/tmedium.inl"
#include "indep/helper/typeTrait.hpp"

namespace by {

#define ME tmay<T>
#define TEMPLATE template <typename T>

    TEMPLATE
    ME::tmay(const T& value): super(value) {}

    TEMPLATE
    ME::tmay(const tmedium<T>& it): super() {
        if(it) set(*it);
    }

    TEMPLATE
    ME::operator T&() { return *get(); }

    TEMPLATE
    ME::operator const T&() const { return *get(); }

    TEMPLATE
    T* ME::operator->() { return get(); }

    TEMPLATE
    T& ME::operator*() { return *get(); }

    TEMPLATE
    ME::operator nbool() const { return has(); }

    TEMPLATE
    nbool ME::has() const { return this->has_value(); }

    TEMPLATE
    T* ME::get() { return has() ? &this->value() : nullptr; }

    TEMPLATE
    void ME::rel() { this->reset(); }

    TEMPLATE
    void ME::set(const T& args) { this->emplace(args); }

#undef TEMPLATE
#undef ME
} // namespace by
