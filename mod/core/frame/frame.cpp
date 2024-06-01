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

    nbool me::pushLocal(nbicontainer* con) { return pushLocal(*con); }
    nbool me::pushLocal(nbicontainer& con) { return pushLocal(*scopes::wrap<scopes>(con)); }
    nbool me::pushLocal(scopes* new1) { return _local.push(*new1); }
    nbool me::pushLocal(scopes& new1) {
        nbool ret = _local.push(new1);
        if(ret && _local.chainLen() == 1 && _obj)
            new1.link(_obj->subs());
        return ret;
    }
    nbool me::pushLocal(const std::string& name, const node& n) {
        scopes& top = *_local.getTop();
        if(nul(top))
            return NAMU_E("couldn't push new node. the top scope is null"), false;

        return top.add(name, n);
    }
    nbool me::pushLocal(const std::string& name, const node* n) {
        return pushLocal(name, *n);
    }

    void me::pushObj(const baseObj& obj) {
        _obj.bind(obj);

        scopes& tail = *_local.getTail();
        if(nul(tail)) return;
        if(nul(obj))
            tail.unlink();
        else
            tail.link(obj.subs());
    }

    node& me::getObjHaving(const node& sub) {
        // TODO: _obj should be array if I support 'with' keyword.
        if(!_obj) return nulOf<node>();

        // TODO: disunite obj scope and subpack scope.
        if(_obj->has(sub)) return *_obj;
        return nulOf<node>();
    }

    namespace {
        thread_local static baseObj* inner = nullptr;
    }

    baseObj& me::_setMe(baseObj& new1) {
        baseObj& ret = *inner;
        inner = &new1;
        return ret;
    }
    baseObj& me::_setMe() {
        return _setMe(nulOf<baseObj>());
    }
    baseObj& me::_getMe() { return *inner; }

    nbicontainer& me::getTop() {
        if(_local.chainLen() > 0)
            return _local.getTop()->getContainer();
        if(_obj)
            return _obj->subs();

        static dumScope inner;
        return inner;
    }

    tstr<scopes> me::popLocal() {
        return _local.pop();
    }
    // I won't provide API for poping a single node from the scope.
    void me::setFunc(baseFunc& new1) { _func.bind(new1); }
    void me::setFunc() { setFunc(nulOf<baseFunc>()); }
    baseFunc& me::getFunc() { return *_func; }
    baseObj& me::getMe() { return *_obj; }

    // node:
    nbicontainer& me::subs() {
        scopes& top = *_local.getTop();
        if(!nul(top)) return top;
        if(_obj) return _obj->subs();

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
        _obj.rel();
        _func.rel();
        _local.rel();
        _ret.rel();
    }
}
