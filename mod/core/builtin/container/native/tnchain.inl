#pragma once

#include "tnchain.hpp"
#include "tnmap.inl"

namespace nm {

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
    ME::tnchain(const std::initializer_list<std::pair<K, V*>>& elems) {
        for(const auto& e: elems)
            _map->add(e.first, *e.second);
    }

    TEMPL
    nbool ME::in(const K& key) const {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            if(e->getContainer().in(key)) return true;
        return false;
    }

    TEMPL
    ncnt ME::len() const {
        ncnt len = 0;
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            len += e->getContainer().len();
        return len;
    }

    TEMPL
    ncnt ME::chainLen() const {
        ncnt len = 0;
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            len++;

        return len;
    }

    TEMPL
    V& ME::get(const K& key) {
        for(tstr<me> e(this); e; e.bind(e->getNext())) {
            V& got = e->getContainer().get(key);
            if(!nul(got)) return got;
        }

        return nulOf<V>();
    }

    TEMPL
    void ME::_getAll(const K& key, narr& tray) const {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            e->getContainer()._getAll(key, tray);
    }

    TEMPL
    nbool ME::add(const K& key, const V& new1) { return getContainer().add(key, new1); }

    TEMPL
    nbool ME::del(const K& key) {
        nbool ret = true;
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            if(e->in(key)) ret = e->getContainer().del(key) ? ret : false;
        return ret;
    }

    TEMPL
    nbool ME::del(const iter& at) {
        const me& owner = (const me&) at.getContainer();

        for(tstr<me> e(this); e; e.bind(e->getNext()))
            if(&e.get() == &owner) return e->getContainer().del(_getMapIterFromChainIter(at));
        return false;
    }

    TEMPL
    nbool ME::del(const iter& from, const iter& last) {
        const me* fromChain = (const me*) &from.getContainer();
        const me *lastChain = (const me*) &last.getContainer()
                                  orRet NM_W("iterator 'end' owned by null chain instance."),
                 false;
        const me* endChain = &lastChain->getNext(); // now, endChain can be null but it's okay.

        me* e = (me*) fromChain;
        nbool ret = true;
        do {
            super& eArr = e->getContainer();
            iter arrBegin = e == fromChain ? _getMapIterFromChainIter(from) : eArr.begin(),
                 arrLast = e == lastChain ? _getMapIterFromChainIter(last) : eArr.end();
            ret = eArr.del(arrBegin, arrLast) ? ret : false;
            e = &e->getNext();
        } while(e != endChain);

        return ret;
    }

    TEMPL
    nbool ME::link(const iter& portion) {
        ME& next = typeProvidable::safeCast<ME>((portion THEN(getContainer()))) orRet false;
        if(&next == this)
            return NM_W("recursive link detected for portion(%s).", (void*) &next), false;

        _next = portion;
        next._prev = _rbeginOfThisChain();
        return true;
    }

    TEMPL
    nbool ME::link(const ME& new1) { return link(new1.begin()); }

    TEMPL
    nbool ME::unlink() {
        ME& next = typeProvidable::safeCast<ME>((_next THEN(getContainer())));
        if(!nul(next)) next._prev.rel();
        _next.rel();
        return true;
    }

    TEMPL
    ME& ME::getTail() {
        me* ret = this;
        while(ret && !ret->_next.isEnd())
            ret = (ME*) &ret->_next.getContainer();
        return *ret;
    }

    TEMPL
    void ME::onCloneDeep(const clonable& from) {
        me& rhs = typeProvidable::safeCast<me>(from);
        _map.bind(*(super*) rhs._map->cloneDeep());

        me* e = this;
        const me* next = &rhs.getNext();
        while(next) {
            e->link(*new me(*(super*) next->getContainer().cloneDeep()));
            e = &e->getNext();
            next = &next->getNext();
        }
    }

    TEMPL
    ME* ME::wrap(const super& toShallowWrap) { return wrap<ME>(toShallowWrap); }

    TEMPL
    ME* ME::cloneChain(const super& until) const {
        tstr<me> e(getNext());
        ME* ret = new ME(this->getContainer());
        ME* retElem = ret;
        while(e) {
            ME* new1 = new ME(e->getContainer());
            retElem->link(*new1);
            retElem = new1;

            if(&e->getContainer() == &until) break;
            e.bind(typeProvidable::safeCast<me>(e->_next.getContainer()));
        }

        return ret;
    }

    TEMPL
    ME* ME::cloneChain(const me& until) const {
        return cloneChain(nul(until) ? nulOf<super>() : until.getContainer());
    }

    TEMPL
    ME* ME::cloneChain() const { return cloneChain(nulOf<me>()); }

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
    ME& ME::getNext() { return typeProvidable::safeCast<ME>(_next.getContainer()); }

    TEMPL
    ME& ME::getPrev() { return typeProvidable::safeCast<ME>(_prev.getContainer()); }

    TEMPL
    typename ME::iteration* ME::_onMakeIteration(const K& key, nbool isReversed) const {
        me* unconst = const_cast<me*>(this);
        return new chainIteration(isReversed ? unconst->_getLastChain() : *unconst, key,
            isReversed);
    }

    TEMPL
    typename ME::iter& ME::_getMapIterFromChainIter(const iter& wrapper) {
        if(!wrapper._iteration->getType().template isSub<chainIteration>()) return nulOf<iter>();
        chainIteration& cast = (chainIteration&) *wrapper._iteration orNul(iter);

        return cast._iter;
    }

    TEMPL
    ME& ME::_getLastChain() {
        tnchain* ret = this;
        while(ret && !nul(ret->_next.getContainer()))
            ret = (tnchain*) &ret->_next.getContainer();
        return *ret;
    }

    TEMPL
    typename ME::iter ME::_rbeginOfThisChain() {
        return iter(new chainIteration(*this, nulOf<K>(), true));
    }

#undef ME
#undef TEMPL
#undef SUPER
} // namespace nm
