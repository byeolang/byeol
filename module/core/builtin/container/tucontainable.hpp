#pragma once

#include "core/ast/param.hpp"
#include <type_traits>

namespace by {

    class node;
    template <typename T, typename R, typename RSquare> class tnucontainer;
    template <typename T, typename TACTIC> class tnarr;
    typedef class _nout tnarr<node, strTactic> narr;
    template <typename K, typename V> class tbicontainable;

    /// @remark tucontainable has API treating iter ref and element as its parameter.
    template <typename T, typename R = T*, typename RSquare = T&> class tucontainable {
        BY_ME(tucontainable)
        static constexpr nbool _IS_POINTER = std::is_pointer_v<R>;

    public:
#include "core/builtin/container/iter/uiter.hpp"

    public:
        virtual ~tucontainable();

        // len:
        virtual ncnt len() const = 0;

        nbool in(const T& it) const;
        nbool in(const T* it) const BY_SIDE_FUNC(in);
        nbool in(std::function<nbool(const T&)> l) const;
        template <typename T1> nbool in(std::function<nbool(const T1&)> l) const;

        nbool isEmpty() const;

        // get:
        template <typename T1> std::conditional_t<_IS_POINTER, T1*, tmay<T1>> get(std::function<nbool(const T1&)> l) {
            for(auto e = begin(); e; ++e) {
                if constexpr(_IS_POINTER) {
                    T1& val = e->template cast<T1>() OR_CONTINUE;
                    if(!l(val)) continue;
                    return &val;
                } else {
                    T1& val = e.get().template cast<T1>() OR_CONTINUE;
                    if(!l(val)) continue;
                    return tmay<T1>(val);
                }
            }

            if constexpr(_IS_POINTER) return nullptr;
            else return tmay<T1>();
        }

        std::conditional_t<_IS_POINTER, R, tmay<R>> get(std::function<nbool(const T&)> l) { return this->get<T>(l); }

        template <typename T1>
        std::conditional_t<_IS_POINTER, const T1*, tmay<T1>> get(std::function<nbool(const T1&)> l) const BY_CONST_FUNC(get(l))
        std::conditional_t<_IS_POINTER, const R, tmay<R>> get(std::function<nbool(const T&)> l) const BY_CONST_FUNC(get(l))

        template <typename T1> tnarr<T1, strTactic> getAll(std::function<nbool(const T1&)> l) const;
        tnarr<T, strTactic> getAll(std::function<nbool(const T&)> l) const;

        template <typename T1> void each(std::function<nbool(T1&)> l);
        void each(std::function<nbool(T&)> l);
        template <typename T1> void each(std::function<nbool(const T1&)> l) const BY_CONST_FUNC(each(l))
        void each(std::function<nbool(const T&)> l) const BY_CONST_FUNC(each(l))

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
        virtual nbool set(const iter& at, const T* new1) BY_SIDE_FUNC(new1, set(at, *new1), false);

        // add:
        virtual nbool add(const iter& at, const T& new1) = 0;
        nbool add(const iter& at, const T* new1) BY_SIDE_FUNC(new1, add(at, *new1), false);
        nbool add(std::initializer_list<const T*> elems);
        nbool add(const T& new1);
        nbool add(const T* it) BY_SIDE_FUNC(add);
        virtual void add(const iter& here, const iter& from, const iter& to) = 0;
        void add(const iter& from, const iter& to);
        void add(const iter& here, me& rhs);
        void add(const me& rhs);
        void add(const me* rhs) BY_SIDE_FUNC(rhs, add(*rhs), void());

        template <typename E>
        ncnt add(const typename tucontainable<E>::iter& from, const typename tucontainable<E>::iter& to) {
            static_assert(areBaseOfT<T, E>::value, "given type 'E' is not subtype of 'T'");
            int ret = 0;
            for(auto e = from; e != to; ++e)
                if(add(*e)) ret++;
            return ret;
        }

        template <typename E> ncnt add(const tucontainable<E>& rhs) { return add(rhs.begin(), rhs.end()); }

        template <typename K, typename V> ncnt add(const tbicontainable<K, V>& rhs);

        // del:
        /// delete last element if exists.
        nbool del();
        nbool del(const T& it);
        nbool del(const T* it) BY_SIDE_FUNC(del);
        virtual nbool del(const iter& it) = 0;
        virtual nbool del(const iter& from, const iter& end) = 0;
        nbool del(const me& rhs);
        nbool del(const me* rhs) BY_SIDE_FUNC(rhs, del(*rhs), false);

        // etc:
        virtual void rel() = 0;

    protected:
        virtual iteration* _onMakeIteration(ncnt step, nbool isReversed) const = 0;
    };

    typedef tucontainable<node> ucontainable;
} // namespace by
