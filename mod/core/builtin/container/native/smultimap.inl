#pragma once

#include "smultimap.hpp"

namespace nm {

#define TEMPL template <typename K, typename V>
#define ME smultimap<K, V>

    TEMPL
    ME::wrap::wrap(V&& newValue): _value(newValue) {}

    TEMPL
    ME::wrap::wrap(const wrap& rhs): _key(rhs._key), _value(rhs._value) {}

    TEMPL
    ME::wrap::wrap(wrap&& rhs): _key(rhs._key), _value(rhs._value) {}

    TEMPL
    const K& ME::wrap::getKey() const { return *_key; }

    TEMPL
    V& ME::wrap::getVal() { return _value; }

    TEMPL
    const V& ME::wrap::getVal() const { return _value; }

    TEMPL
    void ME::wrap::clear() {
        _key = nullptr;
        _prev = _next = this;
    }

    TEMPL
    typename ME::wrap& ME::wrap::operator=(wrap&) { return *this; }

    TEMPL
    typename ME::wrap& ME::wrap::operator=(wrap&&) { return *this; }

    TEMPL
    ME::iterator::iterator(const smultimap* owner, const wrap* pair, nbool isReversed):
        _owner(owner), _wrap(pair), _isReversed(isReversed) {}

    TEMPL
    V& ME::iterator::operator*() { return getVal(); }

    TEMPL
    V* ME::iterator::operator->() { return &getVal(); }

    TEMPL
    typename ME::iterator& ME::iterator::operator++() {
        if(nul(_owner)) return *this;

        _wrap = _isReversed ? _wrap->_prev : _wrap->_next;
        return *this;
    }

    TEMPL
    typename ME::iterator ME::iterator::operator++(int) {
        iterator ret = *this;
        operator++();
        return ret;
    }

    TEMPL
    typename ME::iterator& ME::iterator::operator--() {
        _wrap = _isReversed ? _wrap->_next : _wrap->_prev;
        return *this;
    }

    TEMPL
    typename ME::iterator ME::iterator::operator--(int) {
        iterator ret = *this;
        operator--();
        return ret;
    }

    TEMPL
    bool ME::iterator::isEnd() const { return _wrap == &_owner->_end; }

    TEMPL
    const K& ME::iterator::getKey() const { return _wrap->getKey(); }

    TEMPL
    const V& ME::iterator::getVal() const { return *_wrap; }

    TEMPL
    V& ME::iterator::getVal() { return ((wrap*) _wrap)->getVal(); }

    TEMPL
    bool ME::iterator::operator!=(const iterator& rhs) const { return _wrap != rhs._wrap; }

    TEMPL
    bool ME::iterator::operator==(const iterator& rhs) const { return _wrap == rhs._wrap; }

    TEMPL
    ME::filteredIterator::filteredIterator(const smultimap* owner, const wrap* pair,
        nbool isReversed, const K& key):
        iterator(owner, pair, isReversed), _key(&key) {}

    TEMPL
    typename ME::iterator& ME::filteredIterator::operator++() { return _step(false); }

    TEMPL
    typename ME::iterator& ME::filteredIterator::operator--() { return _step(true); }

    TEMPL
    typename ME::iterator& ME::filteredIterator::_step(nbool isReversed) {
        do {
            isReversed ? super::operator--() : super::operator++();
            if(this->isEnd()) break;
        } while(this->getKey() != *_key);
        return *this;
    }

    TEMPL
    ncnt ME::size() const { return _map.size(); }

    TEMPL
    typename ME::iterator ME::begin() const { return iterator(this, _end._next, false); }

    TEMPL
    typename ME::iterator ME::end() const { return iterator(this, &_end, false); }

    TEMPL
    typename ME::filteredIterator ME::begin(const K& key) const { return _begin(key, false); }

    TEMPL
    typename ME::iterator ME::rbegin() const { return iterator(this, _end._prev, true); }

    TEMPL
    typename ME::iterator ME::rend() const { return iterator(this, &_end, true); }

    TEMPL
    typename ME::filteredIterator ME::rbegin(const K& key) const { return _begin(key, true); }

    TEMPL void ME::insert(const K& key, V&& val) {
        auto e = _map.insert(typename stlMap::value_type(key, wrap(std::forward<V>(val))));
        if(e == _map.end()) return;
        e->second._key = &e->first;

        _link(e->second);
    }

    TEMPL
    void ME::erase(const K& key) {
        auto range = _map.equal_range(key);
        for(auto e = range.first; e != range.second;)
            e = _erase(e);
    }

    TEMPL
    void ME::erase(const iterator& it) {
        auto range = _map.equal_range(it.getKey());
        for(auto e = range.first; e != range.second; ++e)
            if(&(e->second) == it._wrap) return _erase(e), void();
        // not found.
    }

    TEMPL
    void ME::erase(const iterator& from, const iterator& to) {
        for(auto e = from; e != to && e != end(); ++e)
            erase(e);
    }

    TEMPL
    typename ME::iterator ME::find(const K& key) const {
        auto e = begin();
        auto endIter = end();
        while(e != endIter) {
            if(e.getKey() == key) break;
            ++e;
        }
        return e;
    }

    TEMPL
    void ME::clear() {
        _end.clear();
        _map.clear();
    }

    TEMPL
    typename ME::stlMap::iterator ME::_erase(const typename stlMap::iterator& e) {
        if(e == _map.end()) return _map.end(); // not found.

        _unlink(e->second);
        return _map.erase(e);
    }

    TEMPL
    void ME::_link(wrap& newTail) {
        newTail._next = &_end;
        auto*& curTail = _end._prev;
        curTail->_next = &newTail;
        newTail._prev = curTail;
        curTail = &newTail;
    }

    TEMPL
    void ME::_unlink(wrap& toDelete) {
        toDelete._prev->_next = toDelete._next;
        toDelete._next->_prev = toDelete._prev;
    }

    TEMPL
    typename ME::filteredIterator ME::_begin(const K& key, nbool isReversed) const {
        auto ret = filteredIterator(this, &_end, isReversed, key);
        ++ret;
        return ret;
    }

#undef TEMPL
#undef ME
} // namespace nm
