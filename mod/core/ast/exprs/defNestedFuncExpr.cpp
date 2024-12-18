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
        for(auto e = fr.getScopeRegisters().rbegin(); e != fr.getScopeRegisters().rend(); ++e) {
            if(e->owner) break;
            ret->add(*e->s);
        }
        return ret;
    }

    func& me::_getOrigin() { return *_org; }

    str me::getEval() const { return _org->getEval(); }
} // namespace nm
