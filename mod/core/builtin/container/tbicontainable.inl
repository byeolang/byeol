#pragma once

#include "../../ast/node.hpp"
#include "iter/biter.inl"
#include "native/tnarr.inl"
#include "tbicontainable.hpp"

namespace nm {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>

    TEMPL
    ME::~tbicontainable() {}

    TEMPL
    V& ME::operator[](const K& key) { return get(key); }

    TEMPL
    const V& ME::operator[](const K& key) const { return get(key); }

    TEMPL
    ncnt ME::isEmpty() const { return len() <= 0; }

    TEMPL
    nbool ME::in(const V& val) const {
        return !nul(get([&](const K&, const V& elem) { return &elem == &val; }));
    }

    TEMPL
    nbool ME::in(std::function<nbool(const K& key, const V& val)> l) const { return in<V>(l); }

    TEMPL
    template <typename V1> nbool ME::in(std::function<nbool(const K& key, const V1& val)> l) const {
        for(auto e = begin(); e; ++e) {
            V1& val = e->template cast<V1>() orContinue;
            if(l(e.getKey(), val)) return true;
        }
        return false;
    }

    TEMPL
    template <typename V1> V1& ME::get() {
        return get<V1>([](const K&, const V1&) { return true; });
    }

    TEMPL
    template <typename V1> V1& ME::get(const K& key) { return get(key).template cast<V1>(); }

    TEMPL
    template <typename V1> V1& ME::get(std::function<nbool(const K&, const V1&)> l) {
        for(auto e = begin(); e; ++e) {
            V1& val = e.getVal().template cast<V1>();
            if(nul(val) || !l(e.getKey(), val)) continue;
            return val;
        }

        return nulOf<V1>();
    }

    TEMPL
    V& ME::get(std::function<nbool(const K&, const V&)> l) { return this->get<V>(l); }

    TEMPL
    tnarr<V> ME::getAll(const K& key) const {
        narr ret;
        _getAll(key, ret);
        return ret;
    }

    TEMPL
    template <typename V1> tnarr<V1> ME::getAll() const {
        return getAll<V1>([](const K&, const V1&) { return true; });
    }

    TEMPL
    template <typename V1> tnarr<V1> ME::getAll(std::function<nbool(const K&, const V1&)> l) const {
        tnarr<V1> ret;
        for(auto e = begin(); e; ++e) {
            const V1& val = e.getVal().template cast<V1>();
            if(nul(val) || !l(e.getKey(), val)) continue;

            ret.add(val);
        }

        return ret;
    }

    TEMPL
    tnarr<V> ME::getAll(std::function<nbool(const K&, const V&)> l) const {
        return this->getAll<V>(l);
    }

    TEMPL
    template <typename V1> void ME::each(std::function<nbool(const K&, V1&)> l) {
        for(auto e = begin(); e; ++e) {
            V1& val = e.getVal().template cast<V1>() orContinue;
            if(!l(e.getKey(), val)) break;
        }
    }

    TEMPL
    void ME::each(std::function<nbool(const K&, V&)> l) { this->each<node>(l); }

    TEMPL
    typename ME::iter ME::begin() const { return iterate(0, true); }

    TEMPL
    typename ME::iter ME::begin(const K& key) const { return iterate(key, true); }

    TEMPL
    typename ME::iter ME::rbegin() const { return riterate(0, true); }

    TEMPL
    typename ME::iter ME::rbegin(const K& key) const { return riterate(key, true); }

    TEMPL
    typename ME::iter ME::end() const { return iterate(len(), true); }

    TEMPL
    typename ME::iter ME::rend() const { return riterate(len(), true); }

    TEMPL
    typename ME::iter ME::last() const { return iterate(len() - 1); }

    TEMPL
    typename ME::iter ME::iterate(ncnt step) const { return iterate(step, false); }

    TEMPL
    typename ME::iter ME::iterate(ncnt step, nbool isBoundary) const {
        auto* e = _onMakeIteration(nulOf<K>(), false, isBoundary);
        e->next(step);

        return iter(e);
    }

    TEMPL
    typename ME::iter ME::iterate(const K& key) const { return iterate(key, false); }

    TEMPL
    typename ME::iter ME::iterate(const K& key, nbool isBoundary) const {
        if(nul(key)) return iterate(0);
        auto* e = _onMakeIteration(key, false, isBoundary);
        if(!e->isEnd() && e->getKey() != key) e->next(1);

        return iter(e);
    }

    TEMPL
    typename ME::iter ME::riterate(ncnt step) const { return riterate(step, false); }

    TEMPL
    typename ME::iter ME::riterate(ncnt step, nbool isBoundary) const {
        auto* e = _onMakeIteration(nulOf<K>(), true, isBoundary);
        e->next(step);

        return iter(e);
    }

    TEMPL
    typename ME::iter ME::riterate(const K& key, nbool isBoundary) const {
        if(nul(key)) return riterate(0);
        auto* e = _onMakeIteration(key, true, isBoundary);
        if(!e->isEnd() && e->getKey() != key) e->next(1);

        return iter(e);
    }

    TEMPL
    typename ME::iter ME::riterate(const K& key) const { return riterate(key, false); }

    TEMPL
    nbool ME::add(const K& key, const V* val) { return add(key, *val); }

    TEMPL
    ncnt ME::add(const iter& from, const iter& to) {
        int ret = 0;
        for(iter e = from; e != to; ++e)
            if(add(e.getKey(), e.getVal())) ret++;
        return ret;
    }

    TEMPL
    ncnt ME::add(const tbicontainable& rhs) { return add(rhs.begin(), rhs.end()); }

    TEMPL
    nbool ME::del(const tbicontainable& rhs) { return del(rhs.begin(), rhs.end()); }

#undef ME
#undef TEMPL
} // namespace nm
