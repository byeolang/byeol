#include "core/ast/baseFunc.hpp"

#include "core/builtin/primitive/nByte.hpp"
#include "core/builtin/primitive/nInt.hpp"
#include "core/frame/frame.hpp"
#include "core/frame/frameInteract.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/type/as/ases.hpp"
#include "core/ast/params.hpp"
#include "core/ast/closure.hpp"

namespace by {

    BY(DEF_ME(baseFunc), DEF_VISIT())

    me::baseFunc(const modifier& mod): _mod(mod) {}

    scope& me::subs() { return dumScope::singleton(); }

    priorType me::prioritize(const args& a) const {
        BY_DI("%s.prioritize(%s)", *this, a.asStr());
        const params& ps = getParams();
        WHEN(a.len() != ps.len()).ret(NO_MATCH);

        int n = 0;
        priorType max = EXACT_MATCH; // begining from lv0.
        for(const auto& e: a) {
            str t = e.getEval() OR.warn("t == null").ret(NO_MATCH);
            str p = ps[n++].getOrigin().as<node>() OR.ret(NO_MATCH);
            WHEN(!t->isComplete()).ret(NO_MATCH);

            // overloading priority algorithm:
            //  each subs can be categorized into 3 level of priority.
            //  the priority against func call will be computed maximum priority value between the
            //  argument and its paramter. the lower value of 'max', the more check needed.
            priorType newP = _prioritize(*p, *t);
            max = newP > max ? newP : max;
            WHEN(max == NO_MATCH).ret(NO_MATCH);
        }

        return max;
    }

    priorType me::_prioritize(const node& param, const node& arg) const {
        WHEN(arg.getType() == param.getType()).ret(EXACT_MATCH);
        WHEN(_isNatureNumber(param) && _isNatureNumber(arg)).ret(NUMERIC_MATCH);
        WHEN(arg.isImpli(param)).ret(IMPLICIT_MATCH);

        return NO_MATCH;
    }

    void me::_setSrc(const src& newSrc) { _src.bind(newSrc); }

    nbool me::isFuncButNotClosure(const node& n) {
        return n.isSub<baseFunc>() && !n.isSub<closure>();
    }

    ntype& me::_getType() { return (ntype&) getType(); }

    params& me::getParams() { return ((ntype&) getType()).getParams(); }

    const node* me::getRet() const { return getType().getRet(); }

    const src& me::getSrc() const {
        WHEN(!_src).ret(super::getSrc());
        return *_src;
    }

    const modifier& me::getModifier() const {
        WHEN(_mod).ret(*_mod);
        return super::getModifier();
    }

    nbool me::_isNatureNumber(const node& it) const {
        return it.isSub<nInt>() || it.isSub<nByte>();
    }
} // namespace by
