#include "core/ast/closure.hpp"
#include "core/frame/thread.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/exprs/getExpr.hpp"
#include "core/ast/exprs/defNestedFuncExpr.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/type/typeMaker.hpp"

namespace nm {
    NM(DEF_ME(closure), DEF_VISIT())

    namespace {
        funcMgdType _makeClosureType(const baseFunc& from) {
            const ntype& t = from.getType();
            return typeMaker::make<closure>(t.getParams(), t.getRet());
        }
    }

    me::closure(const baseObj& org, const baseFunc& func):
        _org(org), _func(func), _type(_makeClosureType(func)) {}

    const ntype& me::getType() const { return _type; }

    scope& me::subs() { return _func->subs(); }

    priorType me::prioritize(const args& a) const { return _func->prioritize(a); }

    const modifier& me::getModifier() const { return _func->getModifier(); }

    str me::run(const args& a) {
        NM_I("running closure for %s.%s", *_org, *_func);
        WHEN(!_func).ret(str());

        const args& evaled = a.evalAll(_func->getParams()) OR.ret(str());
        evaled.setMe(*_org);
        return _func->run(evaled);
    }

    const baseObj& me::getOrigin() const { return *_org; }

    const baseFunc& me::getFunc() const { return *_func; }

    params& me::getParams() {
        static dumParams dummy;
        WHEN_NUL(_func).ret(dummy);
        return _func->getParams();
    }

    const node* me::getRet() const {
        return _func TO(getRet());
    }

    const src& me::getSrc() const { return _func ? _func->getSrc() : dumSrc::singleton(); }

    me* me::make(const node& e) {
        if(e.isSub<func>()) return _make((func&) e);
        if(e.isSub<getExpr>()) return _make((getExpr&) e);
        if(e.isSub<defNestedFuncExpr>()) return _make((defNestedFuncExpr&) e);
        return nullptr;
    }

    me* me::_make(const func& e) {
        const baseObj& meObj = thread::get().getNowFrame() TO(getMe()) TO(template cast<baseObj>()) OR.ret(nullptr);

        NM_I("make a closure for %s.%s", meObj, e);
        return new me(meObj, e);
    }

    me* me::_make(const getExpr& e) {
        str mayMe = e._evalMe(true);
        frame* fr = mayMe->cast<frame>();
        tstr<baseObj> meObj =
            (fr ? fr->getMe() TO(template cast<baseObj>()) : mayMe->cast<baseObj>()) OR.ret(nullptr);
        baseFunc& cast = e._onGet(*mayMe) TO(template cast<baseFunc>()) OR.ret(nullptr);

        NM_I("make a closure for %s.%s", meObj, cast.getSrc().getName());
        return new me(*meObj, cast);
    }

    me* me::_make(defNestedFuncExpr& e) {
        return _make(e.run()->cast<func>());
    }
} // namespace nm
