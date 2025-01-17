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
ME::wrap& ME::wrap::operator=(wrap&) { return *this; }

TEMPL
ME::wrap& ME::wrap::operator=(wrap&&) { return *this; }

TEMPL
ME::iterator::iterator(smultimap<K, V>* owner, wrap* pair): _owner(owner), _wrap(pair) {}

TEMPL
ME::wrap& ME::iterator::operator*() { return *_wrap; }

TEMPL
ME::wrap* ME::iterator::operator->() { return _wrap; }

TEMPL
ME::iterator& ME::iterator::operator++() {
    _wrap = _wrap->_next;
    return *this;
}

TEMPL
ME::iterator& ME::iterator::operator--() {
    _wrap = _wrap->_prev;
    return *this;
}

TEMPL
bool ME::iterator::isEnd() const { return _wrap == &_owner->_end; }

TEMPL
const K& ME::iterator::getKey() const { return _wrap->getKey(); }

TEMPL
bool ME::iterator::operator!=(const iterator& rhs) const { return _wrap != rhs._wrap; }

TEMPL
bool ME::iterator::operator==(const iterator& rhs) const { return _wrap == rhs._wrap; }


TEMPL
ME::filteredIterator::filteredIterator(me* owner, wrap* pair, const K& key): iterator(owner, pair), _key(key) {}

TEMPL
ME::iterator& ME::filteredIterator::operator++() {
    while(!this->isEnd()) {
        iterator::operator++();
        if(this->getKey() == _key) break;
    }
    return *this;
}

TEMPL
int ME::len() const { return _map.size(); }

TEMPL
bool ME::in(const K& key) const {
    return _map.find(key) != _map.end();
}

TEMPL
V& ME::get(const K& key) {
    if(!in(key)) return nulOf<V>();

    return *_map.equal_range(key)->first;
}

TEMPL
ME::iterator ME::begin() { return iterator(this, _end._next); }

TEMPL
ME::iterator ME::end() { return iterator(this, &_end); }

TEMPL
ME::filteredIterator ME::begin(const K& key) { return filteredIterator(this, _end._next, key); }

TEMPL
void ME::insert(const K& key, V&& val) {
    auto e = _map.insert(typename stlMap::value_type(key, wrap(std::forward<V>(val))));
    if(e == _map.end()) return;
    e->second._key = &e->first;

    _link(e->second);
}

TEMPL
void ME::erase(const K& key) {
    auto range = _map.equal_range(key);
    for(auto e = range.first; e != range.second ;)
        e = _erase(e);
}

TEMPL
void ME::erase(const iterator& it) {
    auto range = _map.equal_range(it.getKey());
    for(auto e = range.first; e != range.second ; ++e)
        if(&(e->second) == it._wrap) return _erase(e), void();
    // not found.
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

#undef TEMPL
#undef ME
}
