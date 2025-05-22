#pragma once

#include "core/builtin/container/native/tnmap.hpp"
#include "core/ast/node.hpp"
#include "core/builtin/container/native/tnbicontainer.inl"

namespace nm {

#define TEMPL template <typename K, typename V, typename TACTIC>
#define ME tnmap<K, V, TACTIC>

    TEMPL
    ME::tnmap() {}

    TEMPL
    ME::tnmap(const std::initializer_list<std::pair<K, V*>>& elems) {
        for(const auto& e: elems)
            add(e.first, *e.second);
    }

    TEMPL
    nbool ME::in(const K& key) const { return _map.begin(key) != _map.end(); }

    TEMPL
    V* ME::get(const K& key) {
        WHEN(!in(key)).retNul<V>();

        return *_map.begin(key).getVal();
    }

    TEMPL
    typename ME::iteration* ME::_onMakeIteration(const K* key, nbool isReversed, ncnt step,
        nbool) const {
        me* unconst = const_cast<me*>(this);
        auto* ret = new nmapIteration(*unconst, key, isReversed);
        ret->next(step);
        return ret;
    }

    TEMPL
    void ME::_getAll(const K& key, narr& tray) const {
        auto end = _map.end();
        for(auto e = _map.begin(key); e != end; ++e)
            tray.add(*e.getVal());
    }

    TEMPL
    nbool ME::add(const K& key, const V& new1) {
        _map.insert(key, wrap(new1));
        return true;
    }

    TEMPL
    nbool ME::del(const K& key) {
        _map.erase(key);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& at) {
        WHEN(!at.isFrom(*this)).warn("from is not an iterator of this container.").ret(false);
        WHEN(at.isEnd()).warn("at is end of the container. skip function.").ret(false);

        _map.erase(*at._iteration._citer);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& end) {
        WHEN(!from.isFrom(*this) || !end.isFrom(*this))
            .warn("from or end is not an iterator of this container")
            .ret(false);

        _map.erase(from._iteration->_citer, end._iteration->_citer);
        return true;
    }

    TEMPL
    ncnt ME::len() const { return _map.size(); };

    TEMPL
    void ME::rel() { _map.clear(); }

    TEMPL
    void ME::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        rel();
        for(iter e = rhs.begin(); e; ++e)
            add(e.getKey(), (V*) e.getVal().cloneDeep());
    }

#undef TEMPL
#undef ME
} // namespace nm
