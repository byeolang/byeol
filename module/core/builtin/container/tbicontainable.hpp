#pragma once

#include "core/ast/param.hpp"

namespace by {

    class node;
    template <typename K, typename V> class tnbicontainer;
    template <typename T, typename TACTIC> class tnarr;
    typedef class _nout tnarr<node, strTactic> narr;

    /// @remark tbicontainable has API treating iter ref and element as its parameter.
    template <typename K, typename V> class tbicontainable {
        BY_ME(tbicontainable)
        template <typename K1, typename V1, typename defaultContainer> friend class tnchain;

    public:
#include "core/builtin/container/iter/biter.hpp"

    public:
        virtual ~tbicontainable();

        // operator:
        V& operator[](const K& key);

        const V& operator[](const K& key) const;

        // len:
        virtual ncnt len() const = 0;

        ncnt isEmpty() const;

        // has:
        virtual nbool in(const K& key) const = 0;
        nbool in(const K* it) const BY_SIDE_FUNC(in);
        nbool in(const V& val) const;
        nbool in(const V* it) const BY_SIDE_FUNC(in);
        nbool in(std::function<nbool(const K& key, const V& val)> l) const;
        template <typename V1> nbool in(std::function<nbool(const K& key, const V1& val)> l) const;

        // get:
        virtual V* get(const K& key) = 0;
        const V* get(const K& key) const BY_CONST_FUNC(get(key))
        V* get(const K* it) BY_SIDE_FUNC(get);
        template <typename V1> V1* get();
        template <typename V1> const V1* get() const BY_CONST_FUNC(get<V1>())
        template <typename V1> V1* get(const K& key);
        template <typename V1> const V1* get(const K& key) const BY_CONST_FUNC(get<V1>(key))
        template <typename V1> V1* get(const K* it) BY_SIDE_FUNC(get<V1>);
        template <typename V1> const V1* get(const K* key) const BY_CONST_FUNC(get<V1>(key))
        template <typename V1> V1* get(std::function<nbool(const K&, const V1&)> l);
        V* get(std::function<nbool(const K&, const V&)> l);
        template <typename V1>
        const V1* get(std::function<nbool(const K&, const V1&)> l) const BY_CONST_FUNC(get(l))
        const V* get(std::function<nbool(const K&, const V&)> l) const BY_CONST_FUNC(get(l))

        tnarr<V, strTactic> getAll(const K& key) const;
        tnarr<V, strTactic> getAll(const K* it) const BY_SIDE_FUNC(getAll);
        template <typename V1> tnarr<V1, strTactic> getAll() const;
        template <typename V1>
        tnarr<V1, strTactic> getAll(std::function<nbool(const K&, const V1&)> l) const;
        tnarr<V, strTactic> getAll(std::function<nbool(const K&, const V&)> l) const;

        template <typename V1> void each(std::function<nbool(const K&, V1&)> l);
        void each(std::function<nbool(const K&, V&)> l);
        template <typename V1>
        void each(std::function<nbool(const K&, const V1&)> l) const BY_CONST_FUNC(each(l))
        void each(std::function<nbool(const K&, const V&)> l) const BY_CONST_FUNC(each(l))

        // iter:
        iter begin() const;
        iter begin(const K& key) const;
        iter begin(const K* it) const BY_SIDE_FUNC(it, begin(*it), iterate(0));

        iter rbegin() const;
        iter rbegin(const K& key) const;
        iter rbegin(const K* it) const BY_SIDE_FUNC(it, rbegin(*it), riterate(0));

        virtual iter end() const;

        virtual iter rend() const;

        virtual iter last() const;

        iter iterate(ncnt step) const;

        /// @param  step    determines which element the `iter` will point to. If `0` is entered, an `iter` will be
        ///                 created that points to the frontmost element.
        /// @param  isBoundary  determines whether this `iter` belongs to the end boundary of the inner container.
        ///                     for example, if there is a container with len() of 3, and a reversed boundary iter
        ///                     that points to the 2nd element for that container is created, and a chain is formed by
        ///                     linking this iter,
        ///                     when iterating over the chain, the 3rd element of the container will be iterated in
        ///                     reverse order.
        iter iterate(ncnt step, nbool isBoundary) const;
        iter iterate(const K& key) const;
        iter iterate(const K* it) const BY_SIDE_FUNC(it, iterate(*it), begin());
        iter iterate(const K& key, nbool isBoundary) const;
        iter iterate(const K* key, nbool isBoundary) const
            BY_SIDE_FUNC(key, iterate(*key, isBoundary), iterate(0, isBoundary));
        iter riterate(ncnt step) const;
        iter riterate(ncnt step, nbool isBoundary) const;
        iter riterate(const K& key) const;
        iter riterate(const K* it) const BY_SIDE_FUNC(it, riterate(*it), rbegin());
        iter riterate(const K& key, nbool isBoundary) const;
        iter riterate(const K* key, nbool isBoundary) const
            BY_SIDE_FUNC(key, riterate(*key, isBoundary), riterate(0, isBoundary));

        virtual nbool add(const K& key, const V& val) = 0;
        nbool add(const K* key, const V& val) BY_SIDE_FUNC(key, add(*key, val), false);
        nbool add(const K& key, const V* val) BY_SIDE_FUNC(val, add(key, *val), false);
        nbool add(const K* key, const V* val) BY_SIDE_FUNC(key || val, add(*key, *val), false);
        ncnt add(const iter& from, const iter& to);
        ncnt add(const tbicontainable& rhs);
        ncnt add(const tbicontainable* it) BY_SIDE_FUNC(add);

        /// delete all elements matched by given key.
        /// @param key key to be deleted
        virtual nbool del(const K& key) = 0;
        nbool del(const K* it) BY_SIDE_FUNC(del);
        virtual nbool del(const iter& at) = 0;
        nbool del(const iter* it) BY_SIDE_FUNC(del);
        virtual nbool del(const iter& from, const iter& end) = 0;
        nbool del(const iter* from, const iter& end) BY_SIDE_FUNC(from, del(*from, end), false);
        nbool del(const iter& from, const iter* end) BY_SIDE_FUNC(end, del(from, *end), false);
        nbool del(const iter* from, const iter* end) BY_SIDE_FUNC(from && end, del(*from, *end), false);

        nbool del(const tbicontainable& rhs);
        nbool del(const tbicontainable* it) BY_SIDE_FUNC(del);

        // etc:
        virtual void rel() = 0;

    protected:
        virtual iteration* _onMakeIteration(const K* key, nbool isReversed, ncnt step,
            nbool isBoundary) const = 0;
        virtual void _getAll(const K& key, narr& tray) const = 0;
    };

    typedef tbicontainable<std::string, node> bicontainable;
} // namespace by
