// nested class of tbicontainable.inl:
//  this file allows to be refered by 'tbicontainable.inl' file only.
#pragma once

#include "../../../ast/node.hpp"

namespace nm {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>::iter

    TEMPL
    typename ME ME::operator+(ncnt step) {
        next(step);
        return *this;
    }

    TEMPL
    typename ME& ME::operator++() {
        next(1);
        return *this;
    }

    TEMPL
    typename ME ME::operator++(int) {
        me ret = *this;
        next(1);
        return ret;
    }

    TEMPL
    typename ME& ME::operator+=(ncnt step) {
        next(step);
        return *this;
    }

    TEMPL
    V& ME::operator*() { return getVal(); }

    TEMPL
    V* ME::operator->() { return &getVal(); }

    TEMPL
    typename ME& ME::operator=(const me& rhs) {
        _assign(rhs);
        return *this;
    }

    TEMPL
    ME::operator nbool() const { return !isEnd(); }

    TEMPL
    nbool ME::isFrom(const tbicontainable& it) const {
        if(!_step) return false;
        return _step->isFrom(it);
    }

    TEMPL
    nbool ME::isEnd() const {
        if(!_step) return true;
        return _step->isEnd();
    }

    TEMPL
    ncnt ME::next(ncnt step) {
        if(!_step) return false;

        for(int n = 0; n < step; n++) {
            if(_step->next(1) <= 0) return n;
            _nextToMatchParamType();
        }

        return step;
    }

    TEMPL
    const K& ME::getKey() const {
        if(!_step) return nulOf<K>();
        return _step->getKey();
    }

    TEMPL
    V& ME::getVal() {
        if(!_step) return nulOf<V>();
        return _step->getVal();
    }

    TEMPL
    void ME::setVal(const V& new1) {
        if(!_step) return;
        return _step->setVal(new1);
    }

    TEMPL
    tbicontainable<K, V>& ME::getContainer() {
        if(!_step) return nulOf<tbicontainable<K, V> >();
        return _step->getContainer();
    }

    TEMPL
    typename ME& ME::_assign(const me& rhs) {
        _step.bind((iteration*) rhs._step->clone());
        return *this;
    }

    TEMPL
    nbool ME::_onSame(const typeProvidable& rhs) const {
        const me& cast = (const me&) rhs;
        return _step == cast._step;
    }

    TEMPL
    void ME::_nextToMatchParamType() {
        while(!isEnd()) {
            if(!nul(getVal())) return;

            next(1);
        }
    }

#undef ME
#undef TEMPL
} // namespace nm
