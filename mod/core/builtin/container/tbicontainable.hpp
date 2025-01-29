#pragma once

#include "../../ast/param.hpp"

namespace nm {

    class node;
    template <typename K, typename V> class tnbicontainer;
    template <typename T, typename TACTIC> class tnarr;
    typedef class _nout tnarr<node, strTactic> narr;

    /// @remark tbicontainable has API treating iter ref and element as its parameter.
    template <typename K, typename V> class tbicontainable {
        NM_ME(tbicontainable)
        template <typename K1, typename V1, typename defaultContainer> friend class tnchain;

    public:
#include "iter/biter.hpp"

    public:
        virtual ~tbicontainable() {}

        // operator:
        V& operator[](const K& key) { return get(key); }

        const V& operator[](const K& key) const { return get(key); }

        // len:
        virtual ncnt len() const = 0;

        ncnt isEmpty() const { return len() <= 0; }

        // has:
        virtual nbool in(const K& key) const = 0;
        nbool in(const V& val) const;
        nbool in(std::function<nbool(const K& key, const V& val)> l) const;
        template <typename V1> nbool in(std::function<nbool(const K& key, const V1& val)> l) const;

        // get:
        virtual V& get(const K& key) = 0;
        const V& get(const K& key) const NM_CONST_FUNC(get(key))
        template <typename V1> V1& get();
        template <typename V1> const V1& get() const NM_CONST_FUNC(get<V1>())
        template <typename V1> V1& get(const K& key);
        template <typename V1> const V1& get(const K& key) const NM_CONST_FUNC(get<V1>(key))
        template <typename V1> V1& get(std::function<nbool(const K&, const V1&)> l);
        V& get(std::function<nbool(const K&, const V&)> l);
        template <typename V1>
        const V1& get(std::function<nbool(const K&, const V1&)> l) const NM_CONST_FUNC(get(l))
        const V& get(std::function<nbool(const K&, const V&)> l) const NM_CONST_FUNC(get(l))

        tnarr<V, strTactic> getAll(const K& key) const;
        template <typename V1> tnarr<V1, strTactic> getAll() const;
        template <typename V1>
        tnarr<V1, strTactic> getAll(std::function<nbool(const K&, const V1&)> l) const;
        tnarr<V, strTactic> getAll(std::function<nbool(const K&, const V&)> l) const;

        template <typename V1> void each(std::function<nbool(const K&, V1&)> l);
        void each(std::function<nbool(const K&, V&)> l);
        template <typename V1>
        void each(std::function<nbool(const K&, const V1&)> l) const NM_CONST_FUNC(each(l))
        void each(std::function<nbool(const K&, const V&)> l) const NM_CONST_FUNC(each(l))

        // iter:
        iter begin() const { return iterate(0); }

        iter rbegin() const { return riterate(0); }

        virtual iter end() const { return iterate(len()); }

        virtual iter rend() const { return riterate(len()); }

        virtual iter last() const { return iterate(len() - 1); }

        iter iterate(ncnt step) const { return iter(_onMakeIteration(step, false)); }

        iter iterate(const K& key) const { return iter(_onMakeIteration(key, false)); }

        iter riterate(ncnt step) const { return iter(_onMakeIteration(step, true)); }

        iter riterate(const K& key) const { return iter(_onMakeIteration(key, true)); }

        // add:
        virtual nbool add(const K& key, const V& val) = 0;

        nbool add(const K& key, const V* val) { return add(key, *val); }

        ncnt add(const iter& from, const iter& to) {
            int ret = 0;
            for(iter e = from; e != to; ++e)
                if(add(e.getKey(), e.getVal())) ret++;
            return ret;
        }

        ncnt add(const tbicontainable& rhs) { return add(rhs.begin(), rhs.end()); }

        // del:
        /// delete last element if exists.
        virtual nbool del(const K& it) = 0;
        virtual nbool del(const iter& at) = 0;
        virtual nbool del(const iter& from, const iter& end) = 0;

        nbool del(const tbicontainable& rhs) { return del(rhs.begin(), rhs.end()); }

        // etc:
        virtual void rel() = 0;

    protected:
        virtual iteration* _onMakeIteration(ncnt step, nbool isReversed) const = 0;
        virtual iteration* _onMakeIteration(const K& key, nbool isReversed) const = 0;
        virtual void _getAll(const K& key, narr& tray) const = 0;
    };

    typedef tbicontainable<std::string, node> bicontainable;
} // namespace nm
