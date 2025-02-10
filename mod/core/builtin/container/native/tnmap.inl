#pragma once

#include "../../../ast/node.hpp"
#include "tnbicontainer.inl"
#include "tnmap.hpp"

namespace nm {

#define TEMPL template <typename K, typename V, typename TACTIC>
#define ME tnmap<K, V, TACTIC>

    TEMPL
    nbool ME::in(const K& key) const { return _map.begin(key) != _map.end(); }

    TEMPL
    V& ME::get(const K& key) {
        if(!in(key)) return nulOf<V>();

        return *_map.begin(key).getVal();
    }

    TEMPL
    void ME::_getAll(const K& key, narr& tray) const {
        auto end = _map.end();
        for(auto e = _map.begin(key); e != end; ++e)
            tray.add(*e.getVal());
    }

    TEMPL
    nbool ME::add(const K& key, const V& new1) {
        if(nul(new1)) return false;

        _map.insert(key, wrap(new1));
        return true;
    }

    TEMPL
    nbool ME::del(const K& it) {
        _map.erase(it);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& at) {
        if(!at.isFrom(*this)) return NM_W("from is not an iterator of this container."), false;
        if(at.isEnd()) return NM_W("at is end of the container. skip function."), false;

        _map.erase(_getIterationFrom(at)._citer);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& end) {
        if(!from.isFrom(*this) || !end.isFrom(*this))
            return NM_W("from or end is not an iterator of this container"), false;

        _map.erase(_getIterationFrom(from)._citer, _getIterationFrom(end)._citer);
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
