#pragma once

#include "core/ast/node.hpp"
#include "core/builtin/container/iter/biter.inl"
#include "core/builtin/container/native/tnarr.inl"
#include "core/builtin/container/tbicontainable.hpp"

namespace by {

#define TEMPL template <typename K, typename V>
#define ME tbicontainable<K, V>

    TEMPL
    ME::~tbicontainable() {}

    TEMPL
    V& ME::operator[](const K& key) { return *get(key); }

    TEMPL
    const V& ME::operator[](const K& key) const { return *get(key); }

    TEMPL
    ncnt ME::isEmpty() const { return len() <= 0; }

    TEMPL
    nbool ME::in(const V& val) const {
        return get([&](const K&, const V& elem) { return &elem == &val; });
    }

    TEMPL
    nbool ME::in(std::function<nbool(const K& key, const V& val)> l) const { return in<V>(l); }

    TEMPL
    template <typename V1> nbool ME::in(std::function<nbool(const K& key, const V1& val)> l) const {
        for(auto e = begin(); e; ++e) {
            V1& val = e->template cast<V1>() OR_CONTINUE;
            const K& key = e.getKey() OR_CONTINUE;
            WHEN(l(key, val)) .ret(true);
        }
        return false;
    }

    TEMPL
    template <typename V1> V1* ME::get() {
        return get<V1>([](const K&, const V1&) { return true; });
    }

    TEMPL
    template <typename V1> V1* ME::get(const K& key) { return get(key) TO(template cast<V1>()); }

    TEMPL
    template <typename V1> V1* ME::get(std::function<nbool(const K&, const V1&)> l) {
        for(auto e = begin(); e; ++e) {
            V1& val = e.getVal() TO(template cast<V1>()) OR_CONTINUE;
            const K& key = e.getKey() OR_CONTINUE;
            if(!l(key, val)) continue;
            return &val;
        }

        return nullptr;
    }

    TEMPL
    V* ME::get(std::function<nbool(const K&, const V&)> l) { return this->get<V>(l); }

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
            const V1& val = e.getVal() TO(template cast<V1>()) OR_CONTINUE;
            const K& key = e.getKey() OR_CONTINUE;
            if(!l(key, val)) continue;

            ret.add(val);
        }

        return ret;
    }

    TEMPL
    tnarr<V> ME::getAll(std::function<nbool(const K&, const V&)> l) const { return this->getAll<V>(l); }

    TEMPL
    template <typename V1> void ME::each(std::function<nbool(const K&, V1&)> l) {
        for(auto e = begin(); e; ++e) {
            V1& val = e.getVal() TO(template cast<V1>()) OR_CONTINUE;
            const K& key = e.getKey() OR_CONTINUE;
            if(!l(key, val)) break;
        }
    }

    TEMPL
    void ME::each(std::function<nbool(const K&, V&)> l) { this->each<V>(l); }

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
        return iter(_onMakeIteration(nullptr, false, step, isBoundary));
    }

    TEMPL
    typename ME::iter ME::iterate(const K& key) const { return iterate(key, false); }

    TEMPL
    typename ME::iter ME::iterate(const K& key, nbool isBoundary) const {
        auto* e = _onMakeIteration(&key, false, 0, isBoundary);
        const K* eKey = e->getKey();
        if(!e->isEnd() && (!eKey || *eKey != key)) e->next(1);

        return iter(e);
    }

    TEMPL
    typename ME::iter ME::riterate(ncnt step) const { return riterate(step, false); }

    TEMPL
    typename ME::iter ME::riterate(ncnt step, nbool isBoundary) const {
        return iter(_onMakeIteration(nullptr, true, step, isBoundary));
    }

    TEMPL
    typename ME::iter ME::riterate(const K& key) const { return riterate(key, false); }

    TEMPL
    typename ME::iter ME::riterate(const K& key, nbool isBoundary) const {
        auto* e = _onMakeIteration(&key, true, 0, isBoundary);
        if(!e->isEnd() && *e->getKey() != key) e->next(1);

        return iter(e);
    }

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
} // namespace by
