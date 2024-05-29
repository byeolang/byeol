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
        NAMU_DI("run: getting me");
        node& me = getMe();
        if(nul(me)) return NAMU_E("run: no thread found"), str();

        NAMU_DI("run: getting sub. me[%s]", me.getType().getName().c_str());
        str sub = _getSub(me.as<node>(), _args);
        if(!sub) return NAMU_E("_subject.as<node>() returns null"), str();

        NAMU_DI("run: is me frame?");
        frame& fr = me.cast<frame>();
        if(!nul(_args)) {
            _args.setMe(!nul(fr) ? fr.getObjHaving(*sub) : me);
            NAMU_DI("run: setting me on args. args.me[%s]", _args.getMe().getType().getName().c_str());
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

    clonable* me::deepClone() const {
        NAMU_DI("runExpr: deepClone");

        me* ret = (me*) clone();
        if(_me) ret->_me.bind((node*) _me->deepClone());

        ret->_args.rel();
        for(const auto& a : _args)
            ret->_args.add((node*) a.deepClone());

        if(_subject) ret->_subject.bind((node*) _subject->deepClone());

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

        str ret = _subject->as<node>();
        return ret;
    }

    str me::getEval() const {
        str sub = _getSub(getMe().getEval(), nulOf<args>());
        if(!sub) return NAMU_E("_subject.as<node>() returns null"), str();

        const baseFunc& f = sub.cast<baseFunc>();
        str ret = nul(f) ? sub : f.getRet();
        if(!ret)
            return NAMU_E("ret is null"), str();
        return new mockNode(*ret->getEval());
    }
}
