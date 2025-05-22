#pragma once

#include "core/builtin/container/native/smultimap.hpp"

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
    const K* ME::wrap::getKey() const { return _key; }

    TEMPL
    V& ME::wrap::getVal() { return _value; }

    TEMPL
    const V& ME::wrap::getVal() const { return _value; }

    TEMPL
    void ME::wrap::clear() { _prev = _next = this; }

    TEMPL
    typename ME::wrap& ME::wrap::operator=(const wrap&) { return *this; }

    TEMPL
    typename ME::wrap& ME::wrap::operator=(wrap&&) { return *this; }

    TEMPL
    const K& ME::iterator::_getDummyKey() {
        static K inner;
        return inner;
    }

    TEMPL
    ME::iterator::iterator(const smultimap* owner, const wrap* pair, nbool isReversed):
        me(owner, pair, isReversed,  nullptr) {}

    TEMPL
    ME::iterator::iterator(const smultimap* owner, const wrap* pair, nbool isReversed,
        const K& key):
        _owner(owner),
        _wrap(pair),
        _isReversed(isReversed),
        _key(key) {}

    TEMPL
    ME::iterator::iterator(const smultimap* owner, const wrap* pair, nbool isReversed,
        const K* key):
        _owner(owner),
        _wrap(pair),
        _isReversed(isReversed),
        _key(key ? *key :  _getDummyKey()) {}

    TEMPL
    V& ME::iterator::operator*() { return getVal(); }

    TEMPL
    V* ME::iterator::operator->() { return &getVal(); }

    TEMPL
    typename ME::iterator& ME::iterator::operator++() {
        WHEN_NUL(_owner).ret(*this);
        return _step(1, false);
    }

    TEMPL
    typename ME::iterator& ME::iterator::_step(ncnt step, nbool isReversed) {
        const K* key = nullptr;
        for(ncnt n = 0; n < step; ++n)
            do {
                _wrap = isReversed ? (_isReversed ? _wrap->_next : _wrap->_prev) :
                                     (_isReversed ? _wrap->_prev : _wrap->_next);
                if(_key == _getDummyKey()) break;
                key = getKey();
                if(!key) return *this;

            } while(*key != _key);
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
        WHEN_NUL(_owner).ret(*this);
        return _step(1, true);
    }

    TEMPL
    typename ME::iterator ME::iterator::operator--(int) {
        iterator ret = *this;
        operator--();
        return ret;
    }

    TEMPL
    typename ME::iterator ME::iterator::operator+(ncnt step) {
        iterator ret = *this;
        ret._step(step, false);
        return ret;
    }

    TEMPL
    bool ME::iterator::isEnd() const { return _wrap == &_owner->_end; }

    TEMPL
    const K* ME::iterator::getKey() const { return _wrap->getKey(); }

    TEMPL
    V& ME::iterator::getVal() {
        auto& val = _wrap->getVal();
        return (V&) val;
    }

    TEMPL
    bool ME::iterator::operator!=(const iterator& rhs) const { return _wrap != rhs._wrap; }

    TEMPL
    bool ME::iterator::operator==(const iterator& rhs) const { return _wrap == rhs._wrap; }

    TEMPL
    ncnt ME::size() const { return _map.size(); }

    TEMPL
    typename ME::iterator ME::begin() const { return iterator(this, _end._next, false); }

    TEMPL
    typename ME::iterator ME::end() const { return iterator(this, &_end, false); }

    TEMPL
    typename ME::iterator ME::begin(const K& key) const { return _begin(key, false); }

    TEMPL
    typename ME::iterator ME::rbegin() const { return iterator(this, _end._prev, true); }

    TEMPL
    typename ME::iterator ME::rend() const { return iterator(this, &_end, true); }

    TEMPL
    typename ME::iterator ME::rbegin(const K& key) const { return _begin(key, true); }

    TEMPL void ME::insert(const K& key, V&& val) {
        auto e = _map.insert(typename stlMap::value_type(key, wrap(std::forward<V>(val))));
        WHEN(e == _map.end()).ret();
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
        for(auto e = from; e != to && e != end();)
            _erase(e++);
    }

    TEMPL
    void ME::clear() {
        _end.clear();
        _map.clear();
    }

    TEMPL
    typename ME::stlMap::iterator ME::_erase(const typename stlMap::iterator& e) {
        WHEN(e == _map.end()).ret(_map.end()); // not found.

        _unlink(e->second);
        return _map.erase(e);
    }

    TEMPL
    void ME::_erase(const iterator& e) {
        WHEN(e.isEnd()).ret(); // not found.

        const K& key = e._wrap->getKey() OR.ret();
        auto range = _map.equal_range(key);
        for(auto stlE = range.first; stlE != range.second; ++stlE)
            if(&stlE->second == e._wrap) return _erase(stlE), void();
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
    typename ME::iterator ME::_begin(const K& key, nbool isReversed) const {
        auto ret = iterator(this, &_end, isReversed, key);
        ++ret;
        return ret;
    }

#undef TEMPL
#undef ME
} // namespace nm
