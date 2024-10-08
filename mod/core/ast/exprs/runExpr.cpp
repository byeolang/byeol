#include "runExpr.hpp"

#include "../../builtin/primitive/nVoid.hpp"
#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "../mockNode.hpp"
#include "getExpr.hpp"

namespace nm {

    NM(DEF_ME(runExpr), DEF_VISIT())

    me::runExpr(const node& meObj, const node& subject, const args& a):
        _me(meObj), _args(a), _subject(subject) {}

    me::runExpr(const node& meObj, const args& a): _me(meObj), _args(a), _subject(meObj) {}

    str me::run(const args& a) {
        auto addr = platformAPI::toAddrId(this);

        str evaledMe = getMe() THEN(template as<node>());
        if(!evaledMe) return NM_E("%s run: evaledMe is null. no thread found", addr), str();

        NM_DI("%s run: getting sub: me[%s]", addr, evaledMe);
        str sub = _getSub(*evaledMe, _args);
        if(!sub) return NM_E("%s _subject.as<node>() returns null", addr), str();

        NM_DI("%s run: assigning me: me[%s] sub[%s]", addr, evaledMe, sub);
        if(!sub->isSub<baseObj>() && !nul(_args)) { // if sub is a baseObj, this expr will runs ctor
                                                    // of it which doesn't need me obj.
            frame& fr = evaledMe->cast<frame>();
            _args.setMe(!nul(fr) ? fr.getOwner(*sub) : *evaledMe);
            NM_DI("%s run: setting me on args. args.me[%s]", addr, _args THEN(getMe()));
        }

        NM_DI("%s run: running sub with args[%s]", addr, _args.toStr());
        str ret = sub->run(_args);

        NM_DI("%s run: done. ret[%s]", addr, ret);
        _args.setMe(nulOf<baseObj>());
        return ret;
    }

    node& me::getMe() {
        if(!_me) return thread::get()._getNowFrame();
        return *_me;
    }

    const node& me::getSubj() const { return *_subject; }

    node& me::getSubj() { return *_subject; }

    void me::setSubj(const node& new1) { _subject.bind(new1); }

    args& me::getArgs() { return _args; }

    const args& me::getArgs() const { return _args; }

    void me::onCloneDeep(const clonable& from) {
        const me& rhs = (const me&) from;

        if(rhs._me) _me.bind((node*) rhs._me->cloneDeep());
        _args.onCloneDeep(rhs._args);
        if(rhs._subject) _subject.bind((node*) rhs._subject->cloneDeep());
    }

    void me::setMe(const node& newMe) { _me.bind(newMe); }

    str me::_getSub(str me, const args& a) const {
        if(!me) return NM_E("me Obj == null"), str();
        if(!_subject) return NM_E("_subject as node == null"), str();

        getExpr& cast = _subject->cast<getExpr>();
        if(!nul(cast)) cast.setMe(*me);

        return _subject->as<node>();
    }

    str me::getEval() const {
        const node& me = getMe() orRet str();
        str sub =
            _getSub(me.getEval(), nulOf<args>()) orRet NM_E("_subject.as<node>() returns null"),
            str();

        const baseFunc& f = sub.cast<baseFunc>();
        str ret = (nul(f) ? sub : f.getRet()) orRet NM_E("ret is null"), str();
        return new mockNode(*ret->getEval());
    }
} // namespace nm
