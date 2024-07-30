#pragma once

#include "../../ast/param.hpp"

namespace nm {

    class node;
    template <typename T, typename R = T&> class tnucontainer;
    template <typename T, typename TACTIC> class tnarr;
    typedef class _nout tnarr<node, strTactic> narr;
    template <typename K, typename V> class tbicontainable;

    /// @remark tucontainable has API treating iter ref and element as its parameter.
    template <typename T, typename R = T&> class tucontainable {
        NM_ME(tucontainable)

    public:
#include "iter/uiter.hpp"

    public:
        virtual ~tucontainable() {}

        // len:
        virtual ncnt len() const = 0;

        nbool isEmpty() const { return len() <= 0; }

        // get:
        template <typename T1> T1& get(std::function<nbool(const T1&)> l);
        R get(std::function<nbool(const T&)> l);
        template <typename T1>
        const T1& get(std::function<nbool(const T1&)> l) const NM_CONST_FUNC(get(l))
        const R get(std::function<nbool(const T&)> l) const NM_CONST_FUNC(get(l))

        template <typename T1> tnarr<T1, strTactic> getAll(std::function<nbool(const T1&)> l) const;
        tnarr<T, strTactic> getAll(std::function<nbool(const T&)> l) const;

        template <typename T1> void each(std::function<nbool(T1&)> l);
        void each(std::function<nbool(T&)> l);
        template <typename T1>
        void each(std::function<nbool(const T1&)> l) const NM_CONST_FUNC(each(l))
        void each(std::function<nbool(const T&)> l) const NM_CONST_FUNC(each(l))

        // iter:
        iter begin() const {
            static iter (me::*specifier)(ncnt) const = &me::iterate;
            return (this->*specifier)(0);
        }

        virtual iter end() const {
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

        nbool has(const T& it) const {
            for(const T& elem: *this)
                if(it == elem) return true;
            return false;
        }

        virtual iter last() const {
            if(len() <= 0) return end();
            static iter (me::*specifier)(ncnt) const = &me::iterate;
            return (this->*specifier)(len() - 1);
        }

        iter iterate(ncnt step) const { return iter(_onMakeIteration(step)); }

        iter iterate(const T& it) const {
            for(iter e = begin(); e; ++e)
                if(&e.get() == &it) return iter(e);

            return iter();
        }

        // set:
        virtual nbool set(const iter& at, const T& new1) = 0;

        nbool set(const iter& at, const T* new1) { return set(at, *new1); }

        // add:
        virtual nbool add(const iter& at, const T& new1) = 0;

        nbool add(const iter& at, const T* new1) { return add(at, *new1); }

        nbool add(std::initializer_list<const T*> elems) {
            nbool ret = false;
            for(auto* elem: elems)
                ret = add(elem);
            return ret;
        }

        nbool add(const T* new1) { return add(*new1); }

        nbool add(const T& new1) { return add(end(), new1); }

        virtual void add(const iter& here, const iter& from, const iter& to) = 0;

        void add(const iter& from, const iter& to) { return add(end(), from, to); }

        void add(const iter& here, me& rhs) { return add(here, rhs.begin(), rhs.end()); }

        void add(const me& rhs) { return add(end(), rhs.begin(), rhs.end()); }

        template <typename E>
        ncnt add(const typename tucontainable<E>::iter& from,
            const typename tucontainable<E>::iter& to) {
            static_assert(areBaseOfT<T, E>::value, "given type 'E' is not subtype of 'T'");
            int ret = 0;
            for(auto e = from; e != to; ++e)
                if(add(*e)) ret++;
            return ret;
        }

        template <typename E> ncnt add(const tucontainable<E>& rhs) {
            return add(rhs.begin(), rhs.end());
        }

        template <typename K, typename V> ncnt add(const tbicontainable<K, V>& rhs);

        // del:
        /// delete last element if exists.
        nbool del() {
            static iter (me::*specifier)(ncnt) const = &me::iterate;
            return del((this->*specifier)(len() - 1));
        }

        nbool del(const T* it) { return del(*it); }

        nbool del(const T& it) {
            static iter (me::*specifier)(const T&) const = &me::iterate;
            return del((this->*specifier)(it));
        }

        virtual nbool del(const iter& it) = 0;
        virtual nbool del(const iter& from, const iter& end) = 0;

        nbool del(const tucontainable& rhs) { return del(rhs.begin(), rhs.end()); }

        // etc:
        virtual void rel() = 0;

    protected:
        virtual iteration* _onMakeIteration(ncnt step) const = 0;
    };

    typedef tucontainable<node> ucontainable;
} // namespace nm
