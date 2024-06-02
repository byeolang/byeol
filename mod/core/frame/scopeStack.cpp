#include "scopeStack.hpp"

namespace namu {

    NAMU_DEF_ME(scopeStack)

    tstr<scopes>& me::getHead() {
        return _head;
    }

    tstr<scopes>& me::getTail() {
        return _tail;
    }

    ncnt me::len() const {
        return _head ? _head->len() : 0;
    }

    ncnt me::chainLen() const {
        return _head ? _head->chainLen() : 0;
    }

    nbool me::addHead(scopes& new1) {
        if(!_tail)
            _tail.bind(new1);
        if(_head)
            new1.link(*_head);
        NAMU_DI("localStack.addHead(Chain(%x))", &new1);
        return _head.bind(new1);
    }

    nbool me::addTail(scopes& new1) {
        if(!_head) return addHead(new1);
        if(_tail)
            _tail->_next.bind(new1);
        NAMU_DI("localStack.pushTail(Chain(%x))", &new1);
        return _tail.bind(new1);
    }

    tstr<scopes> me::pop() {
        if(!_head) return tstr<scopes>();

        tstr<scopes> ret(_head);
        _head.bind(_head->getNext());
        if(!_head)
            _tail.rel();
        NAMU_DI("localStack.pop(%x), .next=%x", &ret.get(), &_head.get());
        return ret;
    }

    void me::rel() {
        _head.rel();
        _tail.rel();
    }
}
