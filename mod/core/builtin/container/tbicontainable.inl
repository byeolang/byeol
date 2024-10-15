#pragma once

#include "../../ast/node.hpp"
#include "iter/biter.inl"
#include "native/tnarr.inl"
#include "tbicontainable.hpp"

namespace nm {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>

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

#undef ME
#undef TEMPL
} // namespace nm
