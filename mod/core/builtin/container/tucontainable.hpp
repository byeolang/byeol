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
        virtual ~tucontainable();

        // len:
        virtual ncnt len() const = 0;

        nbool in(const T& t) const;
        nbool in(std::function<nbool(const T&)> l) const;
        template <typename T1> nbool in(std::function<nbool(const T1&)> l) const;

        nbool isEmpty() const;

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
        iter begin() const;
        iter rbegin() const;

        virtual iter end() const;
        virtual iter rend() const;

        virtual iter last() const;

        iter iterate(ncnt step) const;
        iter iterate(const T& it) const;

        iter riterate(ncnt step) const;
        iter riterate(const T& it) const;

        // set:
        virtual nbool set(const iter& at, const T& new1) = 0;

        nbool set(const iter& at, const T* new1);

        // add:
        virtual nbool add(const iter& at, const T& new1) = 0;

        nbool add(const iter& at, const T* new1);
        nbool add(std::initializer_list<const T*> elems);
        nbool add(const T* new1);
        nbool add(const T& new1);
        virtual void add(const iter& here, const iter& from, const iter& to) = 0;
        void add(const iter& from, const iter& to);
        void add(const iter& here, me& rhs);
        void add(const me& rhs);

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
        nbool del();
        nbool del(const T* it);
        nbool del(const T& it);
        virtual nbool del(const iter& it) = 0;
        virtual nbool del(const iter& from, const iter& end) = 0;
        nbool del(const tucontainable& rhs);

        // etc:
        virtual void rel() = 0;

    protected:
        virtual iteration* _onMakeIteration(ncnt step, nbool isReversed) const = 0;
    };

    typedef tucontainable<node> ucontainable;
} // namespace nm
