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
#define ME tmatches<T>

    TEMPLATE
    ME::tmatches(): _type(NO_MATCH) {}

    TEMPLATE
    nbool ME::isMatched() const { return this->len() == 1; }

    TEMPLATE
    T& ME::get() { return this->get(0); }

    TEMPLATE
    priorType ME::getPriorType() const {
        return _type;
    }

    TEMPLATE
    void ME::_setPriorType(priorType new1) {
        _type = new1;
    }

#undef ME
#define ME tpriorities<T>

    TEMPLATE ME::tpriorities() {}
    TEMPLATE ME::tpriorities(const node& elem) { this->add(new tprior<T>(elem, EXACT_MATCH, 0)); }

    TEMPLATE
    tmatches<T> ME::getMatches() const {
        // assume that this container already got sorted to priority.
        // sub node at index 0 is always highest priority.
        tmatches<T> ret;
        ncnt len = this->len();
        if(len <= 0) return ret;

        const tprior<T>* sample = nullptr;
        this->each([&](const auto& p) {
            if(nul(sample)) sample = &p;
            if(!sample->isSamePrecedence(p)) return false;
            return ret.add(*p.elem);
        });

        if(ret.isMatched())
            ret._setPriorType(sample->type);
        return ret;
    }

    TEMPLATE
    tstr<T> ME::getMatch() {
        auto matches = getMatches();
        if(!matches.isMatched()) return tstr<T>();

        NAMU_DI("priorities.getMatch[%s]", matches[0].getType().getName().c_str());
        return tstr<T>(matches[0]);
    }

    TEMPLATE
    ME ME::split(priorType by) const {
        ME ret;
        priorType p = NO_MATCH;
        for(const auto& e : *this) {
            if(p == NO_MATCH)
                p = e.lv;
            else if(p != e.lv)
                break;
            ret.add(e);
        }
        return ret;
    }

#undef ME
#undef TEMPLATE
}
