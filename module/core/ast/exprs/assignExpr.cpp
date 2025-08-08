#include "core/ast/exprs/assignExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/obj.hpp"

namespace by {

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
        getExpr& get = _lhs TO(template cast<getExpr>()) OR.ret(scope::iter());

        // TODO: elementExpr
        str ased = get TO(getMe()) TO(template as<node>()) OR.exErr(COULDNT_GET_ITER_FOR_LHS, get.getName()).ret(iter());
        frame* fr = ased->cast<frame>();
        str got = get.as<node>();
        scope& subs = (fr ? fr->getScopeHaving(got.get()) : &ased->subs()) OR.exErr(FRAME_DOESNT_HAVE_SCOPE_MATCHING_THIS_TYPE, got.get()).ret(iter());
        return subs.iterate(get.getName());
    }
} // namespace by
