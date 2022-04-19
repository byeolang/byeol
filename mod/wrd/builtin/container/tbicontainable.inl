#pragma once

#include "tbicontainable.hpp"
#include "iter/biter.inl"
#include "../../ast/node.hpp"
#include "native/tnarr.inl"

namespace wrd {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>

    TEMPL
    template <typename V1>
    V1& ME::get(std::function<wbool(const K&, const V1&)> l) const {
        for(auto e=begin(); e ;++e) {
            const V1& val = e.getVal().template cast<V1>();
            if(nul(val) || !l(e.getKey(), val)) continue;
                return (V1&) val;
        }

        return nulOf<V1>();
    }

    TEMPL
    template <typename V1>
    tnarr<V1> ME::getAll(std::function<wbool(const K&, const V1&)> l) const {
        tnarr<V1> ret;
        for(auto e=begin(); e ;++e) {
            const V1& val = e.getVal().template cast<V1>();
            if(nul(val) || !l(e.getKey(), val)) continue;

            ret.add(val);
        }

        return ret;
    }

    TEMPL
    V& ME::get(std::function<wbool(const K&, const V&)> l) const {
        return this->get<V>(l);
    }

    TEMPL
    narr ME::getAll(std::function<wbool(const K&, const V&)> l) const {
        return this->getAll<V>(l);
    }

#undef ME
#undef TEMPL
}
