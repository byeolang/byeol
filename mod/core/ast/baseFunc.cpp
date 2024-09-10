#include "baseFunc.hpp"

#include "../builtin/primitive/nByte.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../frame/frame.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "params.hpp"

namespace nm {

    NM(DEF_ME(baseFunc), DEF_VISIT())

    me::funcType::funcType(const baseFunc& owner): _owner(owner) {}

    const me& me::funcType::getOwner() const { return *_owner; }

    params& me::funcType::getParams() {
        static params dummy;
        return _owner ? _owner->getParams() : dummy;
    }

    str me::funcType::getRet() const { return _owner ? _owner->getRet() : str(); }

    me::baseFunc(): _type(*this) {}

    me::baseFunc(const modifier& mod): _mod(mod), _type(*this) {}

    priorType me::prioritize(const args& a) const {
        const params& ps = getParams();
        if(a.len() != ps.len()) return NO_MATCH;

        int n = 0;
        priorType max = EXACT_MATCH; // begining from lv0.
        for(const auto& e: a) {
            str t = e.getEval();
            if(!t) return NM_W("t == null"), NO_MATCH;
            str p = ps[n++].getOrigin().as<node>();
            if(!t->isComplete()) return NO_MATCH;

            // overloading priority algorithm:
            //  each subs can be categorized into 3 level of priority.
            //  the priority against func call will be computed maximum priority value between the
            //  argument and its paramter. the lower value of 'max', the more check needed.
            priorType newP = _prioritize(*p, *t);
            max = newP > max ? newP : max;
            if(max == NO_MATCH) return NO_MATCH;
        }

        return max;
    }

    const ntype& me::getType() const { return _type; }

    priorType me::_prioritize(const node& param, const node& arg) const {
        if(arg.getType() == param.getType()) return EXACT_MATCH;
        if(_isNatureNumber(param) && _isNatureNumber(arg)) return NUMERIC_MATCH;
        if(arg.isImpli(param)) return IMPLICIT_MATCH;

        return NO_MATCH;
    }

    params& me::getParams() {
        static params inner;
        return inner;
    }

    nbool me::setRet(const node& newRet) { return false; }

    const modifier& me::getModifier() const {
        if(_mod) return *_mod;
        return super::getModifier();
    }

    nbool me::_isNatureNumber(const node& it) const {
        return it.isSub<nInt>() || it.isSub<nByte>();
    }
} // namespace nm
