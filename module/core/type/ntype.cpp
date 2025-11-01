#include "core/ast/node.hpp"
#include "core/type/ntype.hpp"
#include "core/ast/exprs/breakExpr.hpp"
#include "core/ast/exprs/nextExpr.hpp"
#include "core/builtin/primitive.hpp"
#include "core/type/as/impliAses.hpp"

namespace by {

    BY_DEF_ME(ntype)

    nbool me::operator==(const type& rhs) const {
        WHEN(!super::operator==(rhs)) .ret(false);
        return isSameSign(rhs);
    }

    nbool me::isImpli(const type& to) const { return _getImpliAses().is(*this, to); }

    nbool me::isImpli(const typeProvidable& to) const { return isImpli(to.getType()); }

    str me::asImpli(const node& from, const type& to) const { return _getImpliAses().as(from, to); }

    nbool me::is(const type& to) const {
        WHEN(isImpli(to)) .ret(true);

        return _getAses().is(*this, to);
    }

    str me::as(const node& from, const type& to) const {
        WHEN(isImpli(to)) .ret(asImpli(from, to));

        return _getAses().as(from, to);
    }

    nbool me::isImmutable() const { return false; }

    nbool me::isSameSign(const type& rhs) const {
        WHEN(getMetaTypeName() != rhs.getMetaTypeName()) .ret(false);

        const ntype& cast = (const ntype&) rhs;

        WHEN(getParams() != cast.getParams()) .ret(false);

        const ntype* ret = getRet() ? &getRet()->getType() : nullptr;
        const ntype* rhsRet = getRet() ? &getRet()->getType() : nullptr;
        WHEN(!ret && !rhsRet) .ret(true);
        return ret == rhsRet;
    }

    const impliAses& me::_getImpliAses() const {
        static impliAses inner;
        return inner;
    }

    const ases& me::_getAses() const {
        static ases inner;
        return inner;
    }

    const ntype* me::deduce(const ntype& r) const { return deduce(*this, r); }

    const ntype* me::deduce(const typeProvidable& r) const { return deduce((const ntype&) r.getType()); }

    const ntype* me::deduce(const ntype& l, const ntype& r) {
        static deducers* inner = nullptr;
        if(!inner) inner = _makeDeducers();

        if(inner->find(&l) != inner->end()) {
            deducer& dd = inner->at(&l);

            if(dd.find(&r) != dd.end()) return dd.at(&r);
        }

        return &_deduceSuperType(l, r);
    }

    me::deducers* me::_makeDeducers() {
        // make deduce table:
#define _X(A) &ttype<A>::get()

        deducers red = {
            {_X(nInt),
             {
                    {_X(nInt), _X(nInt)},
                    {_X(nFlt), _X(nFlt)}, // if nInt + nFlt ==> nFlt
                    {_X(nByte), _X(nInt)},
                    {_X(nBool), _X(nInt)},
                    {_X(breakRet), _X(nInt)},
                    {_X(nextRet), _X(nInt)},
                }},
            {_X(nFlt),
             {
                    {_X(nInt), _X(nFlt)},
                    {_X(nFlt), _X(nFlt)},
                    {_X(nByte), _X(nFlt)},
                    {_X(nBool), _X(nFlt)},
                    {_X(breakRet), _X(nFlt)},
                    {_X(nextRet), _X(nFlt)},
                }},
            {_X(nStr),
             {
                    {_X(nStr), _X(nStr)},
                    {_X(breakRet), _X(nStr)},
                    {_X(nextRet), _X(nStr)},
                }},
            {_X(nByte),
             {
                    {_X(nInt), _X(nInt)},
                    {_X(nByte), _X(nByte)},
                    {_X(nBool), _X(nByte)},
                }},
            {_X(nBool),
             {
                    {_X(nInt), _X(nInt)},
                    {_X(nFlt), _X(nFlt)},
                    {_X(nByte), _X(nByte)},
                    {_X(nBool), _X(nBool)},
                    {_X(breakRet), _X(nBool)},
                    {_X(nextRet), _X(nBool)},
                }},
            {_X(breakRet),
             {
                    {_X(nInt), _X(nInt)},
                    {_X(nBool), _X(nBool)},
                    {_X(nByte), _X(nByte)},
                    {_X(nStr), _X(nStr)},
                    {_X(nFlt), _X(nFlt)},
                }},
            {_X(nextRet),
             {
                    {_X(nInt), _X(nInt)},
                    {_X(nBool), _X(nBool)},
                    {_X(nByte), _X(nByte)},
                    {_X(nStr), _X(nStr)},
                    {_X(nFlt), _X(nFlt)},
                }}
        };

#undef _X

        return new deducers(red);
    }

    const ntype& me::_deduceSuperType(const ntype& l, const ntype& r) {
        //  reducing super type between l & r algorithm:
        WHEN(l == r) .ret(l);

        const types& lAncestor = l.getSupers();
        const types& rAncestor = r.getSupers();
        ncnt lTier = lAncestor.size() - 1, // tier is the index
            rTier = rAncestor.size() - 1;
        ncnt minTier = lTier > rTier ? rTier : lTier;

        //  main:
        for(int n = minTier; n >= 0; n--)
            if(lAncestor[n] == rAncestor[n]) {
                const ntype& ret = (const ntype&) *lAncestor[n];
                if(!ret.isCustom()) break;
                return ret;
            }

        //  returns void if no valid casting found.
        return ttype<nVoid>::get();
    }

    params& me::getParams() {
        static dumParams dummy;
        return dummy;
    }

    const node* me::getRet() const { return nullptr; }

    void me::setRet(const node& new1) {}

    std::string me::createNameWithParams() const {
        std::string params = getParams().toStr();
        WHEN(params.empty()) .ret(getName());

        return getName() + "<" + params + ">";
    }

    const nchar* me::getMetaTypeName() const { return META_TYPENAME.c_str(); }
} // namespace by
