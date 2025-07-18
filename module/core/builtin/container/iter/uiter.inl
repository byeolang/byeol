// nested class of tucontainable.inl:
//  this file allows to be refered by 'tucontainable.inl' file only.
#pragma once

#include "core/ast/node.hpp"

namespace nm {

#define TEMPL template <typename T, typename R, typename RSquare>
#define ME tucontainable<T, R, RSquare>::iter

    TEMPL
    ME::iter(): me(nullptr) {}

    TEMPL
    ME::iter(iteration* newStep): _iteration(newStep) { _nextToMatchParamType(iterable::NEXT); }

    TEMPL
    ME::iter(const me& rhs) { _assign(rhs); }

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
    RSquare ME::operator*() { return (RSquare) _iteration->operator*(); }

    TEMPL
    typename std::remove_reference<R>::type ME::operator->() { return get(); }

    TEMPL
    typename ME& ME::operator=(const me& rhs) {
        _assign(rhs);
        return *this;
    }

    TEMPL
    ME::operator nbool() const { return !isEnd(); }

    TEMPL
    nbool ME::isReversed() const { return _iteration ? _iteration->isReversed() : false; }

    TEMPL
    void ME::rel() { _iteration TO(rel()); }

    TEMPL
    nbool ME::isFrom(const tucontainable& it) const { return _iteration ? _iteration->isFrom(it) : false; }

    TEMPL
    nbool ME::isEnd() const { return _iteration ? _iteration->isEnd() : true; }

    TEMPL
    ncnt ME::_step(typename iterable::iterationType type, ncnt step) {
        WHEN_NUL(_iteration).ret(0);

        for(int n = 0; n < step; n++) {
            if(_iterate(type, step) <= 0) return n;
            _nextToMatchParamType(type);
        }

        return step;
    }

    TEMPL
    ncnt ME::next(ncnt step) { return _step(iterable::NEXT, step); }

    TEMPL
    ncnt ME::prev(ncnt step) { return _step(iterable::PREV, step); }

    TEMPL
    ncnt ME::stepForward(ncnt step) { return _step(iterable::FORWARD, step); }

    TEMPL
    ncnt ME::stepBackward(ncnt step) { return _step(iterable::BACKWARD, step); }

    TEMPL
    R ME::get() { return _iteration ? (R) _iteration->get() : R{}; }

    TEMPL
    tucontainable<T, R, RSquare>* ME::getContainer() { return _iteration ? _iteration->getContainer() : nullptr; }

    TEMPL
    typename ME& ME::_assign(const me& rhs) {
        _iteration.bind(rhs._iteration ? (iteration*) rhs._iteration->clone() : nullptr);
        return *this;
    }

    TEMPL
    nbool ME::_onSame(const typeProvidable& rhs) const {
        const me& cast = (const me&) rhs;
        return _iteration == cast._iteration;
    }

    TEMPL
    void ME::_nextToMatchParamType(typename iterable::iterationType type) {
        while(!isEnd()) {
            WHEN(!typeTrait<R>::isNul(get())).ret();
            _iterate(type, 1);
        }
    }

    TEMPL
    ncnt ME::_iterate(typename iterable::iterationType type, ncnt step) {
        switch(type) {
            case iterable::FORWARD: return _iteration->stepForward(1);
            case iterable::BACKWARD: return _iteration->stepBackward(1);
            case iterable::PREV: return _iteration->prev(1);
            default:
            case iterable::NEXT: return _iteration->next(1);
        }
    }

#undef ME
#undef TEMPL
} // namespace nm
