#pragma once

#include "core/ast/node.hpp"
#include "core/builtin/err/errCode.hpp"
#include "core/builtin/container/native/tnarr.hpp"
#include "core/builtin/container/native/tnucontainer.inl"

namespace by {

    class thread;
    class baseErr;

#define TEMPL template <typename T, typename TACTIC>
#define ME tnarr<T, TACTIC>

    TEMPL
    ME::tnarr() {}

    TEMPL
    ME::tnarr(const me& rhs): super(rhs) {
        add(rhs);
    }

    TEMPL
    ncnt ME::len() const { return _vec.size(); };

    TEMPL
    nbool ME::in(nidx n) const { return 0 <= n && n < len(); }

    TEMPL
    T* ME::get(nidx n) {
        WHEN(!in(n)).exErr(OUT_OF_RANGE, n, len()).ret(nullptr);

        binder& ret = _vec[n];
        return (T*) ret.get();
    }

    TEMPL
    nbool ME::set(const iter& at, const T& new1) {
        narrIteration& cast = _getIteration(at) OR.ret(false);
        WHEN(cast.isEnd()).ret(false);

        return set(cast._n, new1);
    }

    TEMPL
    nbool ME::set(nidx n, const T& new1) {
        WHEN(!in(n)).exErr(OUT_OF_RANGE, n, len()).ret(false);

        return _vec[n].bind(new1);
    }

    TEMPL
    nbool ME::add(const iter& e, const T& new1) {
        narrIteration& cast = _getIteration(e) OR.ret(false);
        return add(cast._n, new1);
    }

    TEMPL
    nbool ME::add(nidx n, const T& new1) {
        WHEN(n < 0 || n > len()).exErr(OUT_OF_RANGE, n, len()).ret(false);

        _vec.insert(_vec.begin() + n, wrap(new1));
        return true;
    }

    TEMPL
    void ME::add(const iter& here, const iter& from, const iter& to) {
        WHEN(!from.isFrom(to.getContainer())).exErr(ITERATOR_NOT_BELONG_TO_CONTAINER).ret();
        const narrIteration& hereCast = _getIteration(here) OR.ret();
        const narrIteration& fromCast = (narrIteration*) from._iteration.get() OR.ret();
        const narrIteration& toCast = (narrIteration*) to._iteration.get() OR.ret();

        WHEN(hereCast._n < 0 || hereCast._n > len())
            .ret(); // if n equals to len(), it means that will be added at end of container.

        auto fromBegin = ((me*) from.getContainer())->_vec.begin();
        _vec.insert(_vec.begin() + hereCast._n, fromBegin + fromCast._n, fromBegin + toCast._n);
    }

    TEMPL
    nbool ME::del(const iter& at) {
        narrIteration& cast = _getIteration(at) OR.ret(false);
        WHEN(cast.isEnd()).ret(false);

        return del(cast._n);
    }

    TEMPL
    nbool ME::del(nidx n) {
        WHEN(!in(n)).exErr(OUT_OF_RANGE, n, len()).ret(false);

        _vec.erase(_vec.begin() + n);
        return true;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& end) {
        narrIteration& endIter = _getIteration(end) OR.ret(false);
        narrIteration& fromIter = _getIteration(from) OR.ret(false);

        nidx fromN = fromIter.isEnd() ? len() - 1 : fromIter._n;
        ncnt cnt = endIter._n - fromN;
        WHEN(cnt <= 0).ret(false);

        for(int n = 0; n < cnt; n++)
            _vec.erase(_vec.begin() + fromN);
        return true;
    }

    TEMPL
    void ME::rel() { _vec.clear(); }

    TEMPL
    void ME::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;
        rel();
        for(const auto& e: rhs)
            add((T*) e.cloneDeep());
    }

    TEMPL
    std::string ME::asStr() const {
        std::string ret;
        nbool first = true;
        for(const auto& e: *this) {
            ret += (first ? "" : ",") + e.getType().getName();
            first = false;
        }

        return ret;
    }

    TEMPL
    typename ME::iteration* ME::_onMakeIteration(ncnt step, nbool isReversed) const {
        me* unconst = const_cast<me*>(this);
        return new narrIteration(*unconst, step, isReversed);
    }

    TEMPL
    typename ME::narrIteration* ME::_getIteration(const iter& it) {
        WHEN(!it.isFrom(*this)).exErr(ITERATOR_NOT_BELONG_TO_CONTAINER).ret(nullptr);
        auto* ret = (narrIteration*) it._iteration.get();
        WHEN_NUL(ret).exErr(ITERATOR_IS_NUL).ret(nullptr);
        return ret;
    }

#undef TEMPL
#undef ME
} // namespace by
