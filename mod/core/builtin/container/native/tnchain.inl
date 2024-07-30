#pragma once

#include "tnchain.hpp"
#include "tnmap.inl"

namespace nm {

#define TEMPL template <typename K, typename V, typename defaultContainer>
#define ME tnchain<K, V, defaultContainer>

    TEMPL
    nbool ME::has(const K& key) const {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            if(e->getContainer().has(key)) return true;
        return false;
    }

    TEMPL
    nbool ME::has(const V& val) const {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            if(e->getContainer().has(val)) return true;
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
            if(e->has(key)) ret = e->getContainer().del(key) ? ret : false;
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
        const me* lastChain = (const me*) &last.getContainer();
        if(nul(lastChain)) return NM_W("iterator 'end' owned by null chain instance."), false;
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
    tstr<ME> ME::link(const super& new1) {
        if(nul(new1)) return tstr<ME>();

        ME& ret = *wrap<ME>(new1);
        link(ret);
        return tstr<ME>(ret);
    }

    TEMPL
    nbool ME::link(const ME& new1) {
        if(nul(new1) || nul(new1.getContainer())) return false;
        if(&new1 == this)
            return NM_W("recursive link detected!! new1(%s) is this(%s).", (void*) &new1,
                       (void*) this),
                   false;

        return _next.bind(new1);
    }

    TEMPL
    nbool ME::unlink() {
        _next.rel();
        return true;
    }

    TEMPL
    ME& ME::getTail() {
        me* ret = this;
        while(ret->_next)
            ret = &ret->_next.get();
        return *ret;
    }

    TEMPL
    clonable* ME::cloneDeep() const {
        me* ret = new me(*(super*) getContainer().cloneDeep(), getNext());
        me* e = ret;
        const me* next = &getNext();
        while(next) {
            e->_next.bind(new me(*(super*) next->getContainer().cloneDeep()));
            e = &e->getNext();
            next = &next->getNext();
        }
        return ret;
    }

    TEMPL
    template <typename T> T* ME::wrap(const super& toShallowWrap) {
        if(nul(toShallowWrap)) return nullptr;
        T* ret = (T*) &toShallowWrap.template cast<T>();
        if(nul(ret)) {
            ret = new T();
            ret->_map.bind(toShallowWrap);
        }

        return ret;
    }

    TEMPL
    ME* ME::cloneChain(const super& until) const {
        tstr<me> e(getNext());
        ME* ret = new ME(this->getContainer());
        ME* retElem = ret;
        while(e) {
            ME* new1 = new ME(e->getContainer());
            retElem->_next.bind(new1);
            retElem = new1;

            if(&e->getContainer() == &until) break;
            e.bind(e->_next.get());
        }

        return ret;
    }

    TEMPL
    ME* ME::cloneChain(const me& until) const {
        return cloneChain(nul(until) ? nulOf<super>() : until.getContainer());
    }

    TEMPL
    void ME::rel() {
        for(tstr<me> e(this); e; e.bind(e->getNext()))
            e->getContainer().rel();
    }

    TEMPL
    tnbicontainer<K, V>& ME::getContainer() { return *_map; }

#undef ME
#undef TEMPL
} // namespace nm
