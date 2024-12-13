#include "closure.hpp"
#include "../frame/thread.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "exprs/getExpr.hpp"
#include "baseFunc.hpp"

namespace nm {
    NM(DEF_ME(closure), DEF_VISIT())

    me::closure(const baseObj& org, const baseFunc& func): _org(org), _func(func) {}

    scope& me::subs() { return _func->subs(); }

    priorType me::prioritize(const args& a) const { return _func->prioritize(a); }

    const ntype& me::getType() const { return _func->getType(); }

    const modifier& me::getModifier() const { return _func->getModifier(); }

    str me::run(const args& a) {
        NM_I("running closure for %s.%s", *_org, *_func);
        if(!_func) return str();

        args evaled;
        func::evalArgs(a, _func->getParams(), [&](const std::string& name, const node& arg) {
            evaled.add(arg);
        });
        evaled.setMe(*_org);
        return _func->run(evaled);
    }

    const baseObj& me::getOrigin() const { return *_org; }

    const baseFunc& me::getFunc() const { return *_func; }

    params& me::getParams() { return _func THEN(getParams()); }

    closure* me::make(const node& e) {
        if(e.isSub<func>()) return _make((func&) e);
        if(e.isSub<getExpr>()) return _make((getExpr&) e);
        return nullptr;
    }

    closure* me::_make(const func& e) {
        const baseObj& meObj =
            thread::get().getNowFrame() THEN(getMe()) THEN(template cast<baseObj>()) orRet nullptr;
        return new me(meObj, e);
    }

    closure* me::_make(const getExpr& e) {
        if(nul(e)) return nullptr;

        str mayMe = e._evalMe(true);
        frame& fr = mayMe->cast<frame>();
        tstr<baseObj> meObj =
            (!nul(fr) ? fr.getMe().cast<baseObj>() : mayMe->cast<baseObj>()) orRet nullptr;
        baseFunc& cast = e._onGet(*meObj).cast<baseFunc>() orRet nullptr;

        NM_I("make a closure for %s.%s", meObj, cast);
        return new me(*meObj, cast);
    }
} // namespace nm
