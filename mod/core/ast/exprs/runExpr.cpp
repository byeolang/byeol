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

        str evaledMe = safeGet(getMe(), as<node>());
        if(!evaledMe) return NM_E("%s run: evaledMe is null. no thread found", addr), str();

        NM_DI("%s run: getting sub: me[%s]", addr, evaledMe);
        str sub = _getSub(*evaledMe, _args);
        if(!sub) return NM_E("%s _subject.as<node>() returns null", addr), str();

        NM_DI("%s run: assigning me: me[%s] sub[%s]", addr, evaledMe, sub);
        if(!sub->isSub<baseObj>() && !nul(_args)) { // if sub is a baseObj, this expr will runs ctor
                                                    // of it which doesn't need me obj.
            frame& fr = evaledMe->cast<frame>();
            _args.setMe(!nul(fr) ? fr.getOwner(*sub) : *evaledMe);
            NM_DI("%s run: setting me on args. args.me[%s]", addr,
                !evaledMe ? "null" : _args.getMe().getType().getName().c_str());
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

    clonable* me::cloneDeep() const {
        NM_DI("%s.cloneDeep()", *this);

        me* ret = (me*) clone();
        if(_me) ret->_me.bind((node*) _me->cloneDeep());

        ret->_args.rel();
        for(const auto& a: _args)
            ret->_args.add((node*) a.cloneDeep());

        if(_subject) ret->_subject.bind((node*) _subject->cloneDeep());

        return ret;
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
        const node& me = getMe();
        if(nul(me)) return str();

        str sub = _getSub(me.getEval(), nulOf<args>());
        if(!sub) return NM_E("_subject.as<node>() returns null"), str();

        const baseFunc& f = sub.cast<baseFunc>();
        str ret = nul(f) ? sub : f.getRet();
        if(!ret) return NM_E("ret is null"), str();
        return new mockNode(*ret->getEval());
    }
} // namespace nm
