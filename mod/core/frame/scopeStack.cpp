#include "scopeStack.hpp"

namespace namu {

    NAMU_DEF_ME(scopeStack)

    tstr<scopes>& me::getTop() {
        return _top;
    }

    tstr<scopes>& me::getTail() {
        return _tail;
    }

    ncnt me::len() const {
        return _top ? _top->len() : 0;
    }

    ncnt me::chainLen() const {
        return _top ? _top->chainLen() : 0;
    }

    nbool me::push(scopes& new1) {
        // bind scope first:
        //  variable 'scope' can be a 'new'ed variable by push(nbicontainer&) func.
        //  the 'chain' class uses binder when it iterates element.
        //  so if you call chainLen(), then our fresh 'scope' variable will be binded
        //  to binder of a iterater.
        //  so when iterator dies, the 'scope' variable also got freed.
        //  to prevent this case, you have to care about the variable if it has been
        //  passed from the heap. or you will encounter the annoying BAD_ACCESS crash.
        //
        //      so REMEMBER. bind it first or put it into a container first.
        tstr<scopes> lifeSpanner(new1);
        ncnt len = new1.chainLen();
        if(len == 0) return true;
        if(len != 1)
            return NAMU_E("can't bind scopes into local stack if it's more than 2."), false;

        if(_top)
            new1.link(*_top);
        else
            _tail.bind(new1);

        NAMU_DI("localStack.push(Chain(%x))", this, &new1);
        return _top.bind(new1);
    }

    tstr<scopes> me::pop() {
        if(!_top) return tstr<scopes>();

        tstr<scopes> ret(_top);
        _top.bind(_top->getNext());
        if(!_top)
            _tail.rel();
        NAMU_DI("localStack.pop(%x), .next=%x", &ret.get(), &_top.get());
        return ret;
    }

    void me::rel() {
        _top.rel();
        _tail.rel();
    }
}
