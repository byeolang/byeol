#include "defNestedFuncExpr.hpp"
#include "../nestedFunc.hpp"
#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defNestedFuncExpr), DEF_VISIT())

    me::defNestedFuncExpr(const func& org): _org(org) {}

    str me::run(const args& a) {
        if(!_org) return NM_E("_org is null"), str();
        frame &fr = thread::get()._getNowFrame() orRet NM_E("frame doesn't exist"), str();

        nestedFunc* new1 = new nestedFunc(*_org, *_cloneLocalScope(fr));
        NM_I("def nested `%s` func in local", *_org);
        fr.addLocal(_org->getSrc().getName(), *new1);
        return new1;
    }

    scope* me::_cloneLocalScope(frame& fr) const {
        scope* ret = new scope();
        for(const auto& sr : fr.getScopeRegisters()) {
            if(sr.owner) break;
            ret->add(*sr.s);
        }
        return ret;
    }

    const func& me::getOrigin() const { return *_org; }

    str me::getEval() const { return _org->getEval(); }
}
