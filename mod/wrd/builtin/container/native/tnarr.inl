#pragma once

#include "tnarr.hpp"
#include "../../../ast/node.hpp"
#include "tnucontainer.inl"

namespace wrd {

#define TEMPL template <typename T, typename TACTIC>
#define ME tnarr<T, TACTIC>

    TEMPL
    T& ME::get(widx n) {
        if(!has(n)) return nulOf<T>();

        binder& ret = _vec[n];
        return (T&) *ret;
    }

    TEMPL
    wbool ME::set(const iter& at, const T& new1) {
        narrIteration& cast = _getIterationFrom(at);
        if(nul(cast)) return false;
        if(cast.isEnd()) return false;

        return set(cast._n, new1);
    }

    TEMPL
    wbool ME::set(const iter& at, const str& new1) {
        narrIteration& cast = _getIterationFrom(at);
        if(nul(cast)) return false;
        if(cast.isEnd()) return false;

        return set(cast._n, new1);
    }

    TEMPL
    wbool ME::set(widx n, const T& new1) {
        if(!has(n)) return false;

        return _vec[n].bind(new1);
    }

    TEMPL
    wbool ME::set(widx n, const str& new1) {
        if(!has(n)) return false;

        return _vec[n] = new1;
    }

    TEMPL
    wbool ME::add(const iter& e, const T& new1) {
        return add(e, wrap(new1));
    }

    TEMPL
    wbool ME::add(const iter& e, const str& new1) {
        if(nul(e) || nul(new1)) return false;
        if(!e.isFrom(*this)) return false;
        narrIteration& cast = (narrIteration&) *e._step;
        if(nul(cast)) return false;

        return add(cast._n, new1);
    }

    TEMPL
    wbool ME::add(widx n, const T& new1) {
        return add(n, wrap(new1));
    }

    TEMPL
    wbool ME::add(widx n, const str& new1) {
        if(n < 0 || n > len()) return false; // if n equals to len(), it means that will be added at end of container.
        if(!nul(new1)) return false;

        _vec.insert(_vec.begin() + n, new1);
        return true;
    }

    TEMPL
    wbool ME::del(const iter& at) {
        narrIteration& cast = _getIterationFrom(at);
        if(nul(cast)) return false;
        if(cast.isEnd()) return false;

        return del(cast._n);
    }

    TEMPL
    wbool ME::del(widx n) {
        if(!has(n)) return false;
        _vec.erase(_vec.begin() + n);
        return true;
    }

    TEMPL
    wcnt ME::del(const iter& from, const iter& end) {
        narrIteration&  endIter = _getIterationFrom(end),
                    &   fromIter = _getIterationFrom(from);
        if(nul(endIter) || nul(fromIter))
            return WRD_E("from(%x) or end(%x) one of these is null.", &endIter, &fromIter), 0;

        widx fromN = fromIter.isEnd() ? len()-1 : fromIter._n;
        wcnt cnt = endIter._n - fromN;
        if(cnt <= 0) return 0;

        for(int n=0; n < cnt ;n++)
            _vec.erase(_vec.begin() + fromN);
        return cnt;
    }

    TEMPL
    wcnt ME::len() const {
        return _vec.size();
    };

    TEMPL
    void ME::rel() {
        _vec.clear();
    }

    TEMPL
    tstr<instance> ME::deepClone() const {
        me* ret = new me();
        for(auto e=this->begin(); e ;e++)
            ret->add((T*) e->clone());

        return tstr<instance>(ret);
    }

#undef TEMPL
#undef ME
}
