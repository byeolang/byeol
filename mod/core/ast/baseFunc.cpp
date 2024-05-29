#include "baseFunc.hpp"
#include "params.hpp"
#include "../frame/frameInteract.hpp"
#include "../visitor/visitor.hpp"
#include "../frame/frame.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../builtin/primitive/nByte.hpp"

namespace namu {

    NAMU(DEF_ME(baseFunc), DEF_VISIT())

    priority me::prioritize(const args& a) const {
        const params& ps = getParams();
        if(a.len() != ps.len()) return NO_MATCH;

        int n = 0;
        priority max = EXACT_MATCH; // begining from lv0.
        for(const auto& e : a) {
            str t = e.getEval();
            if(!t) return NAMU_W("t == null"), NO_MATCH;
            str p = ps[n++].getOrigin().as<node>();
            if(!t->isComplete()) return NO_MATCH;

            // overloading priority algorithm:
            //  each subs can be categorized into 3 level of priority.
            //  the priority against func call will be computed maximum priority value between the argument and its paramter.
            //  the lower value of 'max', the more check needed.
            priority newP = _prioritize(*p, *t);
            max = newP > max ? newP : max;
            if(max == NO_MATCH)
                return NO_MATCH;
        }

        return max;
    }

    namespace {
        static nbool _isNatureNumber(const node& it) {
            return it.isSub<nInt>() || it.isSub<nByte>();
        }
    }


    priority me::_prioritize(const node& param, const node& arg) const {
        if(arg.getType() == param.getType()) return EXACT_MATCH;
        if(_isNatureNumber(param) && _isNatureNumber(arg)) return NUMERIC_MATCH;
        if(arg.isImpli(param)) return IMPLICIT_MATCH;

        return NO_MATCH;
    }

    const params& me::getParams() const {
        static params inner;
        return inner;
    }

    nbool me::setRet(const node& newRet) {
        return false;
    }

    void me::setOwner(baseObj& owner) { _owner.bind(owner); }
    baseObj& me::getOwner() { return *_owner; }
}
