#include "assignExpr.hpp"
#include "../../frame/thread.hpp"
#include "../obj.hpp"
#include "../../visitor/visitor.hpp"

namespace namu {

    NAMU(DEF_ME(assignExpr), DEF_VISIT())

    me::assignExpr(const node& lhs, const node& rhs): _lhs(lhs), _rhs(rhs) {}

    str me::run(const args& a) {
        iter e = _getScopeIterOfLhs(); // e exists. verified.

        str ret = _rhs->as<node>();
        e.setVal(*ret);
        return ret;
    }

    str me::getEval() const {
        return _rhs->getEval();
    }

    const node& me::getLeft() const {
        return *_lhs;
    }

    const node& me::getRight() const {
        return *_rhs;
    }

    clonable* me::cloneDeep() const {
        me* ret = (me*) clone();
        if(_lhs) ret->_lhs.bind((node*) _lhs->cloneDeep());
        if(_rhs) ret->_rhs.bind((node*) _rhs->cloneDeep());
        return ret;
    }

    me::iter me::_getScopeIterOfLhs() {
        if(!_lhs) return iter();

        getExpr& cast = _lhs->cast<getExpr>();
        if(nul(cast)) return iter();
        // TODO: elementExpr

        str ased = cast.getMe().as<node>();
        if(!ased) return NAMU_E("ased == null"), iter();

        iter ret = ased->subs().iterate(cast.getSubName());
        while(ret) {
            if(!nul(ret.getVal<baseObj>()))
                return ret;

            ++ret;
        }

        return iter();
    }
}
