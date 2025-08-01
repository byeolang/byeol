/// @file
#pragma once

#include "core/builtin/container/native/tnbicontainer.hpp"
#include <initializer_list>
#include <utility>
#include "core/builtin/container/native/smultimap.inl"

namespace by {

    template <typename K, typename V, typename TACTIC = strTactic> class tnmap: public tnbicontainer<K, V> {
        typedef tnbicontainer<K, V> _super_;
        BY(CLASS(tnmap, _super_))

    public:
        template <typename K1, typename V1> friend class tmap;
        typedef tstr<V, TACTIC> wrap;
        typedef smultimap<K, wrap> cmap;
        typedef typename cmap::iterator citer;
        typedef std::pair<K, V> cpair;
        typedef typename super::iter iter;
        typedef typename super::iteration iteration;
        friend class nmapIteration;
#include "../iter/nmapIteration.hpp"

    public:
        tnmap();
        tnmap(const std::initializer_list<std::pair<K, V*>>& elems);

        // len:
        ncnt len() const override;

        // has:
        using super::in;
        nbool in(const K& key) const override;

        // get:
        using super::get;
        V* get(const K& key) override;

        // add:
        using super::add;
        nbool add(const K& key, const V& new1) override;

        // del:
        using super::del;
        nbool del(const K& key) override;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;

        // etc:
        void rel() override;

        /// @remark unlike 'clone()' func, this does deepcopy. so, the output isFrom
        ///         completely different instance compared to 'this'.
        ///         however, it doesn't mean that deepcopy for each element.
        void onCloneDeep(const clonable& from) override;

    protected:
        iteration* _onMakeIteration(const K* key, nbool isReversed, ncnt step, nbool isBoundary) const override;

        void _getAll(const K& key, narr& tray) const override;

    private:
        nmapIteration* _getIterationFrom(const iter& it);

    private:
        cmap _map;
    };
} // namespace by
