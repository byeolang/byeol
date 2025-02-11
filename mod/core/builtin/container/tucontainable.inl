#pragma once

#include "../../ast/node.hpp"
#include "iter/uiter.inl"
#include "native/tnarr.inl"
#include "tbicontainable.hpp"
#include "tucontainable.hpp"

namespace nm {

#define TEMPL template <typename T, typename R>
#define ME tucontainable<T, R>

    TEMPL
    ME::~tucontainable() {}

    TEMPL
    nbool ME::in(const T& t) const {
        return in([&](const T& elem) -> nbool { return elem == t; });
    }

    TEMPL
    template <typename T1> nbool ME::in(std::function<nbool(const T1&)> l) const {
        for(auto e = begin(); e; ++e) {
            const T1& val = e->template cast<T1>() orContinue;
            if(l(val)) return true;
        }
        return false;
    }

    TEMPL
    nbool ME::in(std::function<nbool(const T&)> l) const { return this->in<T>(l); }

    TEMPL
    nbool ME::isEmpty() const { return len() <= 0; }

    TEMPL
    template <typename K, typename V> ncnt ME::add(const tbicontainable<K, V>& rhs) {
        static_assert(areBaseOfT<T, V>::value, "given type 'V' is not subtype of 'T'");
        ncnt ret = 0;
        for(auto e = rhs.begin(); e; ++e)
            ret += add(*e);
        return ret;
    }

    TEMPL
    template <typename T1> T1& ME::get(std::function<nbool(const T1&)> l) {
        for(auto e = begin(); e; ++e) {
            T1& val = e->template cast<T1>();
            if(nul(val) || !l(val)) continue;
            return val;
        }

        return nulOf<T1>();
    }

    TEMPL
    R ME::get(std::function<nbool(const T&)> l) { return this->get<T>(l); }

    TEMPL
    template <typename T1> tnarr<T1> ME::getAll(std::function<nbool(const T1&)> l) const {
        tnarr<T1> ret;
        for(auto e = begin(); e; ++e) {
            const T1& val = e->template cast<T1>();
            if(nul(val) || !l(val)) continue;

            ret.add(val);
        }

        return ret;
    }

    TEMPL
    tnarr<T> ME::getAll(std::function<nbool(const T&)> l) const { return this->getAll<T>(l); }

    TEMPL
    template <typename T1> void ME::each(std::function<nbool(T1&)> l) {
        for(auto e = begin(); e; ++e) {
            T1& val = e->template cast<T1>() orContinue;
            if(!l(val)) break;
        }
    }

    TEMPL
    void ME::each(std::function<nbool(T&)> l) { this->each<T>(l); }

    TEMPL
    typename ME::iter ME::begin() const {
        static iter (me::*specifier)(ncnt) const = &me::iterate;
        return (this->*specifier)(0);
    }

    TEMPL
    typename ME::iter ME::rbegin() const {
        static iter (me::*specifier)(ncnt) const = &me::riterate;
        return (this->*specifier)(0);
    }

    TEMPL
    typename ME::iter ME::end() const {
        // why do you need specifier here?:
        //  please refer nseq. it requires tucontainable to contain nint as type parameter.
        //  in this case, we have very similar 'iterate' funcs.
        //      - iterate(ncnt step)
        //      - iterate(const nint& it)
        //
        //  to avoid ambigious error, I used specifier.
        static iter (me::*specifier)(ncnt) const = &me::iterate;
        return (this->*specifier)(len());
    }

    TEMPL
    typename ME::iter ME::rend() const {
        static iter (me::*specifier)(ncnt) const = &me::riterate;
        return (this->*specifier)(len());
    }

    TEMPL
    typename ME::iter ME::last() const {
        if(len() <= 0) return end();
        static iter (me::*specifier)(ncnt) const = &me::iterate;
        return (this->*specifier)(len() - 1);
    }

    TEMPL
    typename ME::iter ME::iterate(ncnt step) const { return iter(_onMakeIteration(step, false)); }

    TEMPL
    typename ME::iter ME::iterate(const T& it) const {
        for(iter e = begin(); e; ++e)
            if(&e.get() == &it) return iter(e);

        return iter();
    }

    TEMPL
    typename ME::iter ME::riterate(ncnt step) const { return iter(_onMakeIteration(step, true)); }

    TEMPL
    typename ME::iter ME::riterate(const T& it) const {
        for(iter e = rbegin(); e; ++e)
            if(&e.get() == &it) return iter(e);

        return iter();
    }

    TEMPL
    nbool ME::set(const iter& at, const T* new1) { return set(at, *new1); }

    TEMPL
    nbool ME::add(const iter& at, const T* new1) { return add(at, *new1); }

    TEMPL
    nbool ME::add(std::initializer_list<const T*> elems) {
        nbool ret = false;
        for(auto* elem: elems)
            ret = add(elem);
        return ret;
    }

    TEMPL
    nbool ME::add(const T* new1) { return add(*new1); }

    TEMPL
    nbool ME::add(const T& new1) { return add(end(), new1); }

    TEMPL
    void ME::add(const iter& from, const iter& to) { return add(end(), from, to); }

    TEMPL
    void ME::add(const iter& here, me& rhs) { return add(here, rhs.begin(), rhs.end()); }

    TEMPL
    void ME::add(const me& rhs) { return add(end(), rhs.begin(), rhs.end()); }

    TEMPL
    nbool ME::del() {
        static iter (me::*specifier)(ncnt) const = &me::iterate;
        return del((this->*specifier)(len() - 1));
    }

    TEMPL
    nbool ME::del(const T* it) { return del(*it); }

    TEMPL
    nbool ME::del(const T& it) {
        static iter (me::*specifier)(const T&) const = &me::iterate;
        return del((this->*specifier)(it));
    }

    TEMPL
    nbool ME::del(const tucontainable& rhs) { return del(rhs.begin(), rhs.end()); }

#undef ME
#undef TEMPL
} // namespace nm
