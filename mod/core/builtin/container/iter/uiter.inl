// nested class of tucontainable.inl:
//  this file allows to be refered by 'tucontainable.inl' file only.
#pragma once

#include "../../../ast/node.hpp"

namespace nm {

#define TEMPL template <typename T, typename R>
#define ME tucontainable<T, R>::iter

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
    R ME::operator*() {
        return get();
    }

    TEMPL
    typename std::remove_reference<R>::type* ME::operator->() {
        return &get();
    }

    TEMPL
    ME::operator nbool() const {
        return !isEnd();
    }

    TEMPL
    nbool ME::isFrom(const tucontainable& it) const {
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

        for(int n=0; n < step ; n++) {
            if(_step->next(1) <= 0) return n;
            _nextToMatchParamType();
        }

        return step;
    }

    TEMPL
    R ME::get() {
        if(!_step) return nulr<R>::get();
        return (R) _step->get();
    }

    TEMPL
    tucontainable<T, R>& ME::getContainer() {
        if(!_step) return nulOf<tnucontainer<T, R>>();
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
            if(!nulr<R>::isNul(get())) return;

            next(1);
        }
    }

#undef ME
#undef TEMPL
}
