#include "core/ast/exprs/evalExpr.hpp"

#include "core/builtin/scalar/nVoid.hpp"
#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/closure.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/internal/coreInternal.hpp"

namespace by {

    BY(DEF_ME(evalExpr), DEF_VISIT())

    me::evalExpr(const node* meObj, const node& subject, const args& a): _me(meObj), _args(a), _subject(subject) {}

    me::evalExpr(const node* meObj, const args& a): _me(meObj), _args(a), _subject(meObj) {}

    str me::eval(const args& a) {
        auto addr = platformAPI::toAddrId(this);
        str evaledMe = getMe() TO(template as<node>()) OR.err("@%s `me` is null. no thread found", addr).ret(str());

        str sub = _getSub(*evaledMe) OR.err("@%s can't find the func to `%s`", addr, evaledMe).ret(str());

        BY_DI("@%s eval: assigning me: me[%s] sub[%s@%s]", addr, evaledMe, sub, sub.get());

        nbool needMe = !sub->isSub<baseObj>() && !sub->isSub<closure>();
        if(needMe) { // if sub is a baseObj, this expr will runs ctor
                     // of it which doesn't need me obj.
            frame* fr = evaledMe->cast<frame>();
            _args.setMe(fr ? fr->getMeHaving(sub.get()) : evaledMe.get());
            BY_DI("@%s eval: setting me on args. args.me[%s]", addr, _args TO(getMe()));
        }

        BY_I("@%s it'll call `%s.%s@%s(%s)", addr, evaledMe, sub->getSrc(), sub.get(), _args.toStr());
        str ret = sub->eval(_args);

        BY_DI("@%s `%s.%s@%s(%s) --returned--> %s`", addr, evaledMe, sub->getSrc(), sub.get(), _args.toStr(), ret);
        _args.setMe(nullptr);
        return ret;
    }

    node* me::getMe() {
        WHEN_NUL(_me).ret(coreInternal::getNowFrame());
        return _me.get();
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

    str me::_getSub(str me) const {
        WHEN(!me) .err("me Obj == null").ret(str());
        WHEN(!_subject) .err("_subject as node == null").ret(str());

        getExpr* cast = _subject->cast<getExpr>();
        if(cast) cast->setMe(*me);

        return _subject->as<node>();
    }

    str me::infer() const {
        const node& me = getMe() OR.err("me is null").ret(str());
        str sub = _getSub(me.infer()) OR.err("_subject.as<node>() returns null").ret(str());
        WHEN(sub->isSub<baseObj>()) .ret(sub->isComplete() ? sub : new mockNode(*sub));

        baseFunc& cast = sub->cast<baseFunc>() OR.err("sub isn't obj or func. returns null").ret(str());
        return new mockNode(cast.getRet());
    }
} // namespace by
