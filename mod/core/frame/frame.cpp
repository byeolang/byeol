#include "frame.hpp"
#include "../ast/obj.hpp"
#include "../ast/baseFunc.hpp"
#include "../visitor/visitor.hpp"
#include "../ast/dumScope.hpp"

namespace namu {
    NAMU(DEF_ME(frame), DEF_VISIT())

    me::frame() {
        _rel();
    }

    me::~frame() {
        _rel();
    }

    nbool me::add(scopes& existing) {
        tstr<scopes> cloned(existing.cloneChain());
        if(!cloned) return false;
        cloned->getTail().link(*_stack);
        _stack = cloned;
        return true;
    }
    nbool me::addLocal(const std::string& name, const node& n) {
        if(!_stack)
            return NAMU_E("couldn't push new node. the top scope is null"), false;
        auto& top = *_stack;
        const node& owner = top.getOwner();
        if(!nul(top.getOwner()))
            return NAMU_E("it's tried to add variable into %s. it's not valid.", owner.getType().getName().c_str()), false;

        return top.add(name, n);
    }

    node& me::getObjHaving(const node& sub) {
        // TODO: _obj should be array if I support 'with' keyword.
        if(!_me) return nulOf<node>();

        // TODO: disunite obj scope and subpack scope.
        if(_me->has(sub)) return *_me;
        return nulOf<node>();
    }

    nbool me::setMe(const baseObj& new1) {
        return _me.bind(new1);
    }
    void me::setMe() {
        _me.rel();
    }
    baseObj& me::getMe() { return *_me; }

    void me::del() {
        if(!_stack) return;

        _stack.bind(_stack->getNext());
    }

    nbool me::setFunc(baseFunc& new1) { return _func.bind(new1); }
    void me::setFunc() { setFunc(nulOf<baseFunc>()); }
    baseFunc& me::getFunc() { return *_func; }

    // node:
    nbicontainer& me::subs() {
        if(_stack) return *_stack;

        static dumScope inner;
        return inner;
    }

    priority me::prioritize(const args& a) const {
        return NO_MATCH;
    }

    str me::run(const args& a) {
        return str();
    }

    void me::rel() {
        _rel();
        super::rel();
    }

    nbool me::setRet(const node& newRet) const {
        return _ret.bind(newRet);
    }

    nbool me::setRet() const {
        _ret.rel();
        return true;
    }

    node& me::getRet() const {
        return *_ret;
    }

    void me::_rel() {
        _me.rel();
        _func.rel();
        _stack.rel();
        _ret.rel();
    }
}
