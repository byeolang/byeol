#include "runExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/primitive/nVoid.hpp"
#include "getExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../mockNode.hpp"

namespace namu {

    NAMU(DEF_ME(runExpr), DEF_VISIT())

    me::runExpr(const node& meObj, const node& subject, const args& a):
            _me(meObj), _args(a), _subject(subject)  {}
    me::runExpr(const node& meObj, const args& a): _me(meObj), _args(a), _subject(meObj) {}

    str me::run(const args& a) {
        node& me = getMe();
        if(nul(me)) return NAMU_E("run: no thread found"), str();

        NAMU_DI("run: getting sub: me[%s]", me.getType().getName().c_str());
        str sub = _getSub(me.as<node>(), _args);
        if(!sub) return NAMU_E("_subject.as<node>() returns null"), str();

        NAMU_DI("run: assigning me: me[%s] sub[%s]", me.getType().getName().c_str(), sub->getType().getName().c_str());
        if(!nul(_args)) {
            frame& fr = me.cast<frame>();
            _args.setMe(!nul(fr) ? fr.getOwner(*sub) : me);
            const node& me = _args.getMe();
            NAMU_DI("run: setting me on args. args.me[%s]", nul(me) ? "null" : me.getType().getName().c_str());
        }

        NAMU_DI("run: running sub with args[%s]", _args.toStr().c_str());
        str ret = sub->run(_args);

        NAMU_DI("run: done. ret[%s]", ret ? ret->getType().getName().c_str() : "null");
        _args.setMe(nulOf<baseObj>());
        return ret;
    }

    node& me::getMe() {
        if(!_me)
            return thread::get()._getNowFrame();
        return *_me;
    }

    const node& me::getSubject() const {
        return *_subject;
    }

    node& me::getSubject() {
        return *_subject;
    }

    void me::setSubject(const node& new1) {
        _subject.bind(new1);
    }

    args& me::getArgs() {
        return _args;
    }

    const args& me::getArgs() const {
        return _args;
    }

    clonable* me::cloneDeep() const {
        NAMU_DI("runExpr: cloneDeep");

        me* ret = (me*) clone();
        if(_me) ret->_me.bind((node*) _me->cloneDeep());

        ret->_args.rel();
        for(const auto& a : _args)
            ret->_args.add((node*) a.cloneDeep());

        if(_subject) ret->_subject.bind((node*) _subject->cloneDeep());

        return ret;
    }

    void me::setMe(const node& newMe) {
        _me.bind(newMe);
    }

    str me::_getSub(str me, const args& a) const {
        if(!me) return NAMU_E("me Obj == null"), str();
        if(!_subject) return NAMU_E("_subject as node == null"), str();

        getExpr& cast = _subject->cast<getExpr>();
        if(!nul(cast))
            cast.setMe(*me);

        return _subject->as<node>();
    }

    str me::getEval() const {
        const node& me = getMe();
        if(nul(me)) return str();

        str sub = _getSub(me.getEval(), nulOf<args>());
        if(!sub) return NAMU_E("_subject.as<node>() returns null"), str();

        const baseFunc& f = sub.cast<baseFunc>();
        str ret = nul(f) ? sub : f.getRet();
        if(!ret)
            return NAMU_E("ret is null"), str();
        return new mockNode(*ret->getEval());
    }
}
