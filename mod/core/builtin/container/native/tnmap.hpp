#pragma once

#include <type/ttype.hpp>
#include "tnbicontainer.hpp"

namespace nm {

    template <typename K, typename V, typename TACTIC = strTactic>
    class tnmap: public tnbicontainer<K, V> {
        typedef tnbicontainer<K, V> _super_;
        NM(CLASS(tnmap, _super_))

    public:
        template <typename K1, typename V1> friend class tmap;
        typedef tstr<V, TACTIC> wrap;
        typedef std::multimap<K, wrap> cmap;
        typedef typename cmap::iterator citer;
        typedef std::pair<K, V> cpair;
        typedef typename super::iter iter;
        typedef typename super::iteration iteration;
        friend class nmapIteration;
#include "../iter/nmapIteration.hpp"

    public:
        tnmap() {}

        // len:
        ncnt len() const override;

        // has:
        nbool in(const K& key) const override;
        nbool in(const V& val) const override;

        // get:
        using super::get;
        V& get(const K& key) override;

        // add:
        using super::add;
        nbool add(const K& key, const V& new1) override;

        // del:
        using super::del;
        nbool del(const K& it) override;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;

        // etc:
        void rel() override;

        /// @remark unlike 'clone()' func, this does deepcopy. so, the output isFrom
        ///         completely different instance compared to 'this'.
        ///         however, it doesn't mean that deepcopy for each element.
        clonable* cloneDeep() const override;

    protected:
        iteration* _onMakeIteration(ncnt step) const override {
            me* unconst = const_cast<me*>(this);
            iteration* ret =
                new nmapIteration(*unconst, unconst->_map.begin(), unconst->_map.end());
            ret->next(step);
            return ret;
        }

        iteration* _onMakeIteration(const K& key) const override {
            me* unconst = const_cast<me*>(this);
            return new nmapIteration(*unconst, unconst->_map.lower_bound(key),
                unconst->_map.upper_bound(key));
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
} // namespace nm
