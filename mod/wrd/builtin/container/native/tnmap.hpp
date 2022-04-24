#pragma once

#include "tnbicontainer.hpp"
#include "../../../ast/node.hpp"

namespace wrd {

    template <typename K, typename V, typename TACTIC = strTactic>
    class tnmap : public tnbicontainer<K, V> {
        typedef tnbicontainer<K, V> __super;
        WRD(CLASS(tnmap, __super))
        template <typename K1, typename V1> friend class tmap;
        typedef tstr<V, TACTIC> wrap;
        typedef std::multimap<K, wrap> cmap;
        typedef typename cmap::iterator citer;
        typedef std::pair<K, V> cpair;
        typedef typename super::iter iter;
        typedef typename super::iteration iteration;

    public:
        friend class nmapIteration;
#include "../iter/nmapIteration.hpp"

    public:
        tnmap() {}

        // len:
        wcnt len() const override;

        // has:
        wbool has(const K& key) const override;

        // get:
        using super::get;
        V& get(const K& key) override;

        // add:
        using super::add;
        wbool add(const K& key, const V& new1) override;

        // del:
        using super::del;
        wcnt del(const K& it) override;
        wbool del(const iter& at) override;
        wcnt del(const iter& from, const iter& end) override;

        // etc:
        void rel() override;

        tstr<instance> deepClone() const override;

    protected:
        iteration* _onMakeIteration(wcnt step) const override {
            me* unconst = const_cast<me*>(this);
            iteration* ret = new nmapIteration(*unconst, unconst->_map.begin());
            ret->next(step);
            return ret;
        }
        void _getAll(const K& key, narr& tray) const override;

    private:
        nmapIteration& _getIterationFrom(const iter& it) {
            if(nul(it)) return nulOf<nmapIteration>();
            if(!it.isFrom(*this)) return nulOf<nmapIteration>();
            return (nmapIteration&) *it._step;
        }

    private:
        cmap _map;
    };

    typedef tnmap<std::string, node> nmap;
}
