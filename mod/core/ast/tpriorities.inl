#pragma once

#include "tpriorities.hpp"

namespace nm {

#define TEMPLATE template <typename T>
#define ME tprior<T>

    TEMPLATE
    ME::tprior(const node& newElem, priorType newType, ncnt newLv): type(newType), lv(newLv) {
        elem.bind(newElem);
    }

    TEMPLATE T* ME::operator->() { return &get(); }

    TEMPLATE T& ME::operator*() { return get(); }

    TEMPLATE T& ME::get() { return elem.get(); }

    TEMPLATE nbool ME::isSamePrecedence(const ME& rhs) const {
        return type == rhs.type && lv == rhs.lv;
    }

#undef ME
#define ME tpriorities<T>

    TEMPLATE
    ME::tpriorities(): _type(priorType::EXACT_MATCH) {}

    TEMPLATE
    nbool ME::isMatched() const { return this->len() == 1; }

    TEMPLATE
    priorType ME::getPriorType() const { return _type; }

    TEMPLATE
    T& ME::get() {
        if(this->len() != 1) return nulOf<T>();

        return this->get(0);
    }

    TEMPLATE
    void ME::_setPriorType(priorType new1) { _type = new1; }

#undef ME
#undef TEMPLATE
} // namespace nm
