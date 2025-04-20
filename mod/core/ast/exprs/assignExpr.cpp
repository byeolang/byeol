#include "core/ast/exprs/assignExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/obj.hpp"

namespace nm {

    NM(DEF_ME(assignExpr), DEF_VISIT())

    me::assignExpr(const node& lhs, const node& rhs): _lhs(lhs), _rhs(rhs) {}

    str me::run(const args& a) {
        iter e = _getScopeIterOfLhs(); // e exists. verified.

        str ret = _rhs->as<node>();
        e.setVal(*ret);
        NM_I("@%s `%s = %s`", this, e.getKey(), *ret);
        return ret;
    }

    str me::getEval() const { return _rhs->getEval(); }

    const node& me::getLeft() const { return *_lhs; }

    const node& me::getRight() const { return *_rhs; }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        if(rhs._lhs) _lhs.bind((node*) rhs._lhs->cloneDeep());
        if(rhs._rhs) _rhs.bind((node*) rhs._rhs->cloneDeep());
    }

    me::iter me::_getScopeIterOfLhs() {
        getExpr& cast = _lhs TO(template cast<getExpr>()) OR.ret(iter());

        // TODO: elementExpr
        str ased = cast TO(getMe()) TO(template as<node>()) OR.err("ased == null").ret(iter());
        frame& fr = ased->cast<frame>();
        if(!nul(fr)) {
            scope& s = fr.getScopeHaving(*cast.as<node>());
            if(!nul(s)) return s.iterate(cast.getName());
        }

        WHEN(!ased).exErr(COULDNT_GET_ITER_FOR_LHS, cast.getName()).ret(iter());

        return ased->subs().iterate(cast.getName());
    }
} // namespace nm
