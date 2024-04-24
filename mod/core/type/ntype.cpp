#include "ntype.hpp"
#include "../ast/node.hpp"
#include "../builtin/primitive.hpp"

namespace namu {

    NAMU_DEF_ME(ntype)

    nbool me::operator==(const type& rhs) const {
        if(!super::operator==(rhs)) return false;

        const ntype& cast = dynamic_cast<const ntype&>(rhs);
        if(nul(cast)) return false;
        // if there is no specified bean type, don't bean type check:
        //  mostly, bean type should specified. except for binder. so if I return false
        //  in this case,
        //      tstr<arr> wrap(new tarr<nInt>())
        //  above code doesn't work.
        if(nul(cast.getBean())) return true;
        if(nul(getBean())) return true;

        return getBean().getType() == cast.getBean().getType();
    }

    nbool me::isImpli(const type& to) const {
        return _getImpliAses().is(*this, to);
    }
    nbool me::isImpli(const typeProvidable& to) const { return isImpli(to.getType()); }

    str me::asImpli(const node& from, const type& to) const {
        return _getImpliAses().as(from, to);
    }

    nbool me::is(const type& to) const {
        if(isImpli(to)) return true;

        return _getAses().is(*this, to);
    }

    str me::as(const node& from, const type& to) const {
        if(isImpli(to))
            return asImpli(from, to);

        return _getAses().as(from, to);
    }

    nbool me::isImmutable() const {
        return false;
    }

    const ases& me::_getImpliAses() const {
        static ases inner;
        return inner;
    }

    const ases& me::_getAses() const {
        static ases inner;
        return inner;
    }

    const ntype& me::deduce(const ntype& r) const {
        return deduce(*this, r);
    }

    const ntype& me::deduce(const ntype& l, const ntype& r) {
        static deducers* inner = nullptr;
        if(nul(inner))
            inner = _makeDeducers();

        if(inner->find(&l) != inner->end()) {
            deducer& dd = inner->at(&l);

            if(dd.find(&r) != dd.end())
                return *dd.at(&r);
        }

        return _deduceSuperType(l, r);
    }

    me::deducers* me::_makeDeducers() {
        // make deduce table:
#define _X(A) &ttype<A>::get()

        deducers red = {
            {_X(nInt), {
                {_X(nInt), _X(nInt)},
                {_X(nFlt), _X(nFlt)}, // if nInt + nFlt ==> nFlt
                {_X(nByte), _X(nInt)},
                {_X(nBool), _X(nInt)},
                {_X(nChar), _X(nChar)},
            }},
            {_X(nFlt), {
                {_X(nInt), _X(nFlt)},
                {_X(nFlt), _X(nFlt)},
                {_X(nByte), _X(nFlt)},
                {_X(nBool), _X(nFlt)}
            }},
            {_X(nStr), {
                {_X(nStr), _X(nStr)},
                {_X(nChar), _X(nStr)},
            }},
            {_X(nByte), {
                {_X(nInt), _X(nInt)},
                {_X(nByte), _X(nByte)},
                {_X(nBool), _X(nByte)},
                {_X(nChar), _X(nChar)},
            }},
            {_X(nBool), {
                {_X(nInt), _X(nInt)},
                {_X(nFlt), _X(nFlt)},
                {_X(nByte), _X(nByte)},
                {_X(nBool), _X(nBool)},
                {_X(nChar), _X(nChar)},
            }},
            {_X(nChar), {
                {_X(nChar), _X(nChar)},
                {_X(nBool), _X(nChar)},
                {_X(nByte), _X(nChar)},
                {_X(nInt), _X(nChar)},
                {_X(nStr), _X(nStr)},
            }},
        };

#undef _X

        return new deducers(red);
    }

    const ntype& me::_deduceSuperType(const ntype& l, const ntype& r) {
        //  reducing super type between l & r algorithm:
        if(nul(l) || nul(r)) return nulOf<ntype>();
        if(l == r) return l;

        const types& lAncestor = l.getSupers();
        const types& rAncestor = r.getSupers();
        ncnt lTier = lAncestor.size() - 1, // tier is the index
             rTier = rAncestor.size() - 1;
        ncnt minTier = lTier > rTier ? rTier : lTier;

        //  main:
        for(int n = minTier; n >= 0 ;n--)
            if(lAncestor[n] == rAncestor[n]) {
                const ntype& ret = (const ntype&) *lAncestor[n];
                if(!ret.isCustom())
                    break;
                return ret;
            }

        // there is no null in namulang:
        //  returns void if no valid casting found.
        return ttype<nVoid>::get();
    }
}
