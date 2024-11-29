#include "defNestedFuncExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defNestedFuncExpr), DEF_VISIT())

    me::defNestedFuncExpr(const func& org): _org(org) {}

    str me::run(const args& a) {
        if(!_org) return NM_E("_org is null"), str();
        frame &fr = thread::get()._getNowFrame() orRet NM_E("frame doesn't exist"), str();

        func* new1 = new func(*new modifier(false, false), (mgdType&) _org->getType(),
            *new scope(*(scope::super*) fr.getLocals().getContainer().clone(), _org->subs()),
            _org->getBlock());

        NM_I("def nested `%s` func in local", *_org);
        fr.addLocal(_org->getSrc().getName(), *new1);
        return new1;
    }

    const func& me::getOrigin() const { return *_org; }

    str me::getEval() const { return _org->getEval(); }
}
