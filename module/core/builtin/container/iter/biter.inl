// nested class of tbicontainable.inl:
//  this file allows to be refered by 'tbicontainable.inl' file only.
#pragma once

#include "core/ast/node.hpp"

namespace by {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>::iter

    TEMPL
    ME::iter(): me(nullptr) {}

    TEMPL
    ME::iter(iteration* newStep): _iteration(newStep) {}

    TEMPL
    ME::iter(const me& rhs) { _assign(rhs); }

    TEMPL
    typename ME ME::operator+(ncnt step) {
        ME ret = *this;
        ret.next(step);
        return ret;
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
    V& ME::operator*() { return *getVal(); }

    TEMPL
    V* ME::operator->() { return getVal(); }

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
    nbool ME::isFrom(const tbicontainable& it) const {
        WHEN(!_iteration) .ret(false);
        return _iteration->isFrom(it);
    }

    TEMPL
    nbool ME::isEnd() const { return _iteration ? _iteration->isEnd() : true; }

    TEMPL
    ncnt ME::_step(typename iterable::iterationType type, ncnt step) {
        WHEN_NUL(_iteration).ret(0);

        for(int n = 0; n < step; n++) {
            if(_iterate(type) <= 0) return n;
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
    const K* ME::getKey() const { return _iteration ? _iteration->getKey() : nullptr; }

    TEMPL
    V* ME::getVal() { return _iteration ? _iteration->getVal() : nullptr; }

    TEMPL
    void ME::setVal(const V& new1) {
        if(_iteration) _iteration->setVal(new1);
    }

    TEMPL
    tbicontainable<K, V>* ME::getContainer() { return _iteration ? _iteration->getContainer() : nullptr; }

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
            WHEN(getVal()) .ret();
            _iterate(type);
        }
    }

    TEMPL
    ncnt ME::_iterate(typename iterable::iterationType type) {
        WHEN_NUL(_iteration).ret(BY_INDEX_ERROR);
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
} // namespace by
