#pragma once

#include "core/builtin/container/native/tnchain.hpp"
#include "core/builtin/container/native/tnmap.inl"

namespace by {

#define TEMPL template <typename K, typename V, typename defaultContainer>
#define ME tnchain<K, V, defaultContainer>
#define SUPER typename ME::super

    TEMPL
    ME::tnchain(): _map(new defaultContainer()) {}

    TEMPL
    ME::tnchain(const super& arr): _map(arr) {}

    TEMPL
    ME::tnchain(const super& org, const me& next): _map(org) { link(next); }

    TEMPL
    ME::tnchain(std::initializer_list<std::pair<K, V*>> elems) {
        for(const auto& e: elems)
            _map->add(e.first, *e.second);
    }

    TEMPL
    nbool ME::in(const K& key) const {
        nbool ret = false;
        this->each([&](const K& elemKey, const V&) { return ret = elemKey != key; });
        return !ret;
    }

    TEMPL
    ncnt ME::len() const {
        ncnt len = 0;
        this->each([&](const K&, const V&) { return len++, true; });
        return len;
    }

    TEMPL
    ncnt ME::chainLen() const {
        ncnt len = 0;
        for(const me* e = this; e; e = e->getNext())
            len++;
        return len;
    }

    TEMPL
    V* ME::get(const K& key) {
        V* ret = nullptr;
        this->each([&](const K& elemKey, V& val) {
            WHEN(elemKey != key).ret(true);
            ret = &val;
            return false;
        });
        return ret;
    }

    TEMPL
    void ME::_getAll(const K& key, narr& tray) const {
        this->each([&](const K& elemKey, const V& val) {
            if(elemKey == key) tray.add(val);
            return true;
        });
    }

    TEMPL
    nbool ME::add(const K& key, const V& new1) { return getContainer().add(key, new1); }

    TEMPL
    nbool ME::del(const K& key) {
        nbool ret = true;
        for(me* e = this; e; e = e->getNext())
            if(!e->getContainer().del(key)) ret = false;
        return ret;
    }

    TEMPL
    nbool ME::del(const iter& at) {
        const me* owner = (const me*) at.getContainer();
        for(me* e = this; e; e = e->getNext()) {
            if(e != owner) continue;
            return e->getContainer().del(_getInnerIter(at));
        }
        return false;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& last) {
        WHEN(from.isReversed() != last.isReversed()).exErr(ITERATORS_ARENT_SAME_DIRECTION).ret(false);
        const me* fromChain = (const me*) from.getContainer();
        const me& lastChain =
            (const me*) last.getContainer() OR.warn("iterator 'end' owned by null chain instance.").ret(false);
        const me* endChain = lastChain.getNext(); // now, endChain can be null but it's okay.

        me* e = (me*) fromChain;
        nbool ret = true;
        do {
            super& eArr = e->getContainer();
            iter innerBegin = _getInnerBeginOfChain(*e, *fromChain, from),
                 innerLast = _getInnerEndOfChain(*e, lastChain, last);
            ret = eArr.del(innerBegin, innerLast) ? ret : false;
            e = e->getNext();
        } while(e != endChain);

        return ret;
    }

    TEMPL
    nbool ME::link(const iter& portion) {
        ME& next = (ME*) (portion TO(getContainer())) OR.ret(false);
        WHEN(&next == this).warn("recursive link detected for portion(%s).", (void*) &next).ret(false);

        _next = portion;
        // this's not reversed to portion iterator:
        // the direction of the `chain iter` is determined by the perspective from which the chain
        // is viewed.
        // if you are looking at a chain from a reversed viewpoint and you link a new
        // chain to that chain from a normal viewpoint, the viewpoint from which the new chain looks
        // at the reversed chain must be reversed.
        //
        // e.g.
        //  A ----reversed--> B  ----normal--> C
        //    <--normal-----     <--reversed--
        //
        // therefore, the value of `next._prev.isReversed()` must be the same as the value of
        // `prev._next.isReversed()`. This ensures that advancing iters on both ends will produce
        // the same result.
        me* prev = getPrev();
        next._prev = _rendOfThisChain(prev ? prev->_next.isReversed() : false);
        return true;
    }

    TEMPL
    nbool ME::link(const ME& new1) { return link(new1.begin()); }

    TEMPL
    nbool ME::unlink() {
        ME* next = (ME*) (_next TO(getContainer()));
        if(next) next->_prev.rel();
        _next.rel();
        return true;
    }

    TEMPL
    ME* ME::getTail() {
        me* ret = this;
        while(ret && ret->_next.getContainer())
            ret = (me*) ret->_next.getContainer();
        return ret;
    }

    TEMPL
    void ME::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;
        _map.bind(*(super*) rhs._map->cloneDeep());

        me* e = this;
        const me* next = rhs.getNext();
        while(next) {
            e->link(*new me(*(super*) next->getContainer().cloneDeep()));
            e = e->getNext();
            next = next->getNext();
        }
    }

    TEMPL
    ME* ME::wrap(const super& toShallowWrap) { return wrap<ME>(toShallowWrap); }

    TEMPL
    ME* ME::cloneChain(const super* until) const {
        tstr<me> e(getNext());
        ME* ret = new ME(this->getContainer());
        ME* retElem = ret;
        while(e) {
            tstr<me> new1(new ME(e->getContainer()));
            retElem->link(*new1);
            retElem = new1.get();

            if(&e->getContainer() == until) break;
            e.bind((me*) e->_next.getContainer());
        }

        return ret;
    }

    TEMPL
    ME* ME::cloneChain(const me* until) const {
        return cloneChain(until ? &until->getContainer() : (const super*) nullptr);
    }

    TEMPL
    ME* ME::cloneChain() const { return cloneChain((const super*) nullptr); }

    TEMPL
    void ME::rel() {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            e->getContainer().rel();
    }

    TEMPL
    tnbicontainer<K, V>& ME::getContainer() { return *_map; }

    TEMPL
    const SUPER& ME::getContainer() const { return *_map; }

    TEMPL
    ME* ME::getNext() { return (ME*) _next.getContainer(); }

    TEMPL
    ME* ME::getPrev() { return (ME*) _prev.getContainer(); }

    TEMPL
    typename ME::iteration* ME::_onMakeIteration(const K* key, nbool isReversed, ncnt step, nbool isBoundary) const {
        me* unconst = const_cast<me*>(this);
        auto* ret = new nchainIteration(isReversed ? unconst->getTail() : unconst, key, isReversed, isBoundary, true);
        ret->next(step);
        ret->_setBoundary(isBoundary);
        return ret;
    }

    TEMPL
    typename ME::iter* ME::_getInnerIter(const iter& outer) {
        nchainIteration& cast = (nchainIteration*) outer._iteration.get() OR.ret(nullptr);
        return &cast._iter;
    }

    TEMPL
    typename ME::iter ME::_getInnerBeginOfChain(me& it, const me& fromChain, const iter& from) {
        me* prev = it.getPrev();
        nbool isReversed = prev ? prev->_next.isReversed() : false;
        WHEN(&it != &fromChain).ret(it.getContainer().begin());
        WHEN(isReversed).ret(it.getContainer().begin());

        auto ret = _getInnerIter(from) OR.ret(this->end());
        return ret;
    }

    TEMPL
    typename ME::iter ME::_getInnerEndOfChain(me& it, const me& lastChain, const iter& last) {
        me* prev = it.getPrev();
        nbool isReversed = !prev ? false : prev->_next.isReversed();
        WHEN(&it != &lastChain).ret(it.getContainer().end());
        WHEN(isReversed).ret(it.getContainer().end());

        auto ret = _getInnerIter(last) OR.ret(this->end());
        return ret;
    }

    TEMPL
    typename ME::iter ME::_rendOfThisChain(nbool isReversed) {
        return iter(new nchainIteration(this, nullptr, isReversed, true, false));
    }

#undef ME
#undef TEMPL
#undef SUPER
} // namespace by
