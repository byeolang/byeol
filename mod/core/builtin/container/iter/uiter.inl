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
    R ME::operator*() { return get(); }

    TEMPL
    typename std::remove_reference<R>::type* ME::operator->() { return &get(); }

    TEMPL
    typename ME& ME::operator=(const me& rhs) {
        _assign(rhs);
        return *this;
    }

    TEMPL
    ME::operator nbool() const { return !isEnd(); }

    TEMPL
    nbool ME::isFrom(const tucontainable& it) const {
        if(!_iteration) return false;
        return _iteration->isFrom(it);
    }

    TEMPL
    nbool ME::isEnd() const {
        if(!_iteration) return true;
        return _iteration->isEnd();
    }

    TEMPL
    ncnt ME::_step(std::function<ncnt(void)> closure, ncnt step) {
        if(!_iteration) return false;

        for(int n = 0; n < step; n++) {
            if(closure() <= 0) return n;
            _nextToMatchParamType();
        }

        return step;
    }

    TEMPL
    ncnt ME::next(ncnt step) {
        return _step([&]()->ncnt { return _iteration->next(1); }, step);
    }

    TEMPL
    ncnt ME::stepForward(ncnt step) {
        return _step([&]()->ncnt { return _iteration->stepForward(1); }, step);
    }

    TEMPL
    ncnt ME::stepBackward(ncnt step) {
        return _step([&]()->ncnt { return _iteration->stepBackward(1); }, step);
    }

    TEMPL
    R ME::get() {
        if(!_iteration) return nulr<R>::get();
        return (R) _iteration->get();
    }

    TEMPL
    tucontainable<T, R>& ME::getContainer() {
        if(!_iteration) return nulOf<tnucontainer<T, R>>();
        return _iteration->getContainer();
    }

    TEMPL
    typename ME& ME::_assign(const me& rhs) {
        _iteration.bind((iteration*) rhs._iteration->clone());
        return *this;
    }

    TEMPL
    nbool ME::_onSame(const typeProvidable& rhs) const {
        const me& cast = (const me&) rhs;
        return _iteration == cast._iteration;
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
} // namespace nm
