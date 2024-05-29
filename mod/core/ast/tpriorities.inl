#pragma once

#include "tpriorities.hpp"

namespace namu {

#define TEMPLATE template <typename T>
#define ME tprior<T>

    TEMPLATE
    ME::tprior(const node& newElem, priority newLv): lv(newLv) {
        elem.bind(newElem);
    }

    TEMPLATE
    ME::tprior(const node& newElem, const node& newOwner, priority newLv): lv(newLv) {
        elem.bind(newElem);
        owner.bind(newOwner);
    }

    TEMPLATE T* ME::operator->() { return &get(); }
    TEMPLATE T& ME::operator*() { return get(); }

    TEMPLATE T& ME::get() { return elem.get(); }

    TEMPLATE nbool ME::isSamePrecedence(const ME& rhs) const {
        return lv == rhs.lv && &owner.get() == &rhs.owner.get();
    }

#undef ME
#define ME tmatches<T>

    TEMPLATE
    ME::tmatches(): _lv(NO_MATCH) {}

    TEMPLATE
    nbool ME::isMatched() const { return this->len() == 1; }

    TEMPLATE
    T& ME::get() { return this->get(0); }

    TEMPLATE
    priority ME::getPriority() const {
        return _lv;
    }

    TEMPLATE
    void ME::_setPriority(priority new1) {
        _lv = new1;
    }

#undef ME
#define ME tpriorities<T>

    TEMPLATE
    ME::tpriorities() {}

    TEMPLATE
    ME::tpriorities(const T& elem) {
        this->add(*new tprior<T>(elem, EXACT_MATCH));
    }

    TEMPLATE
    ME::tpriorities(const T& elem, const node& owner) {
        this->add(*new tprior<T>(elem, owner, EXACT_MATCH));
    }

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
            ret._setPriority(sample->lv);
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
    ME ME::split(priority by) const {
        ME ret;
        priority p = NO_MATCH;
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
