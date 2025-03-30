#include "expr.hpp"

#include "../builtin/container/native/tnmap.inl"
#include "../frame/frame.hpp"
#include "args.hpp"
#include "baseFunc.hpp"
#include "dumScope.hpp"

namespace nm {

    NM_DEF_ME(expr)

    nbool me::exprType::isImpli(const type& to) const { return to.isSub<node>(); }

    str me::exprType::asImpli(const node& from, const type& to) const {
        str ret = ((node&) from).run() TO(asImpli(to)) orRet str();
        return ret;
    }

    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    nbool me::isImpli(const type& to) const { return getEval()->isSub(to); }

    priorType me::prioritize(const args& a) const { return a.len() == 0 ? EXACT_MATCH : NO_MATCH; }

    const src& me::getSrc() const {
        if(!_src) return super::getSrc();
        return *_src;
    }

    void me::_setSrc(const src& newSrc) { _src.bind(newSrc); }
} // namespace nm
