#pragma once

#include "tmay.inl"
#include "tres.hpp"

namespace nm {
    
#define ME tres<T, R>
#define TEMPLATE template <typename T, typename R>

    TEMPLATE
    ME::tres(const T& value): super(value) {}

    TEMPLATE
    R& ME::getErr() {
        if(this->has()) return nulOf<R>();
        return _err.value();
    }

    TEMPLATE
    void ME::rel() {
        _err.reset();
        super::rel();
    }

    TEMPLATE
    void ME::set(const T& arg) {
        super::set(arg);
        _err.reset();
    }

    TEMPLATE
    void ME::setErr(const R& arg) {
        super::rel();
        _err.emplace(arg);
    }

#undef TEMPLATE
#undef ME
}
