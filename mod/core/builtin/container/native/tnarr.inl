#pragma once

#include "tnarr.hpp"
#include "../../../ast/node.hpp"
#include "tnucontainer.inl"
#include "../../../loader/errCode.hpp"
#include "../../../type/exMaker.hpp"

namespace nm {

    class thread;
    struct err;

#define TEMPL template <typename T, typename TACTIC>
#define ME tnarr<T, TACTIC>

    TEMPL
    T& ME::get(nidx n) {
        NM_WHEN(!has(n)).ex(OUT_OF_RANGE, n, len()), nulOf<T>();

        binder& ret = _vec[n];
        return (T&) *ret;
    }

    TEMPL
    nbool ME::set(const iter& at, const T& new1) {
        narrIteration& cast = _getIterationFrom(at);
        if(nul(cast)) return false;
        if(cast.isEnd()) return false;

        return set(cast._n, new1);
    }

    TEMPL
    nbool ME::set(nidx n, const T& new1) {
        if(!has(n)) return false;

        return _vec[n].bind(new1);
    }

    TEMPL
    nbool ME::add(const iter& e, const T& new1) {
        if(nul(e) || nul(new1)) return false;
        if(!e.isFrom(*this)) return false;
        narrIteration& cast = (narrIteration&) *e._step;
        if(nul(cast)) return false;

        return add(cast._n, new1);
    }

    TEMPL
    nbool ME::add(nidx n, const T& new1) {
        if(n < 0 || n > len()) return false; // if n equals to len(), it means that will be added at end of container.
        if(nul(new1)) return false;

        _vec.insert(_vec.begin() + n, wrap(new1));
        return true;
    }

    TEMPL
    void ME::add(const iter& here, const iter& from, const iter& to) {
        if(!from.isFrom(to.getContainer())) return;
        const narrIteration& hereCast = _getIterationFrom(here);
        const narrIteration& fromCast = (narrIteration&) *from._step;
        const narrIteration& toCast = (narrIteration&) *to._step;
        if(nul(hereCast) || nul(fromCast) || nul(toCast)) return;

        if(hereCast._n < 0 || hereCast._n > len()) return; // if n equals to len(), it means that will be added at end of container.

        auto fromBegin = ((me&) from.getContainer())._vec.begin();
        _vec.insert(_vec.begin() + hereCast._n, fromBegin + fromCast._n, fromBegin + toCast._n);
    }

    TEMPL
    nbool ME::del(const iter& at) {
        narrIteration& cast = _getIterationFrom(at);
        if(nul(cast)) return false;
        if(cast.isEnd()) return false;

        return del(cast._n);
    }

    TEMPL
    nbool ME::del(nidx n) {
        if(!has(n)) return false;
        _vec.erase(_vec.begin() + n);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& end) {
        narrIteration&  endIter = _getIterationFrom(end),
                    &   fromIter = _getIterationFrom(from);
        if(nul(endIter) || nul(fromIter))
            return NM_E("from(%s) or end(%s) one of these is null.", (void*) &endIter, (void*) &fromIter), false;

        nidx fromN = fromIter.isEnd() ? len()-1 : fromIter._n;
        ncnt cnt = endIter._n - fromN;
        if(cnt <= 0) return false;

        for(int n=0; n < cnt ;n++)
            _vec.erase(_vec.begin() + fromN);
        return true;
    }

    TEMPL
    ncnt ME::len() const {
        return _vec.size();
    };

    TEMPL
    void ME::rel() {
        _vec.clear();
    }

    TEMPL
    clonable* ME::cloneDeep() const {
        me* ret = new me();
        for(auto e=this->begin(); e ;e++)
            ret->add((T*) e->cloneDeep());

        return ret;
    }

#undef TEMPL
#undef ME
}
