#include "baseFunc.hpp"

#include "../builtin/primitive/nByte.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../frame/frame.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "../type/as/ases.hpp"
#include "params.hpp"
#include "closure.hpp"

namespace nm {

    NM(DEF_ME(baseFunc), DEF_VISIT())

    me::baseFunc(const modifier& mod): _mod(mod) {}

    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    priorType me::prioritize(const args& a) const {
        NM_DI("%s.prioritize(%s)", *this, nul(a) ? "()" : a.asStr());
        const params& ps = getParams();
        WHEN(a.len() != ps.len()).ret(NO_MATCH);

        int n = 0;
        priorType max = EXACT_MATCH; // begining from lv0.
        for(const auto& e: a) {
            str t = e.getEval() OR_RET NM_W("t == null"), NO_MATCH;
            str p = ps[n++].getOrigin().as<node>();
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
        WHEN_NUL(param, arg).ret(NO_MATCH);
        WHEN(arg.getType() == param.getType()).ret(EXACT_MATCH);
        WHEN(_isNatureNumber(param) && _isNatureNumber(arg)).ret(NUMERIC_MATCH);
        WHEN(arg.isImpli(param)).ret(IMPLICIT_MATCH);

        return NO_MATCH;
    }

    void me::_setSrc(const src& newSrc) { _src.bind(newSrc); }

    nbool me::isFuncButNotClosure(const node& n) {
        WHEN(nul(n)).ret(false);
        return n.isSub<baseFunc>() && !n.isSub<closure>();
    }

    ntype& me::_getType() { return (ntype&) getType(); }

    params& me::getParams() { return ((ntype&) getType()).getParams(); }

    const node& me::getRet() const { return getType().getRet(); }

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
} // namespace nm
