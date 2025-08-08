#include "core/ast/expr.hpp"

#include "core/builtin/container/native/tnmap.inl"
#include "core/frame/frame.hpp"
#include "core/ast/args.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/dumScope.hpp"

namespace by {

    NM_DEF_ME(expr)

    nbool me::exprType::isImpli(const type& to) const { return to.isSub<node>(); }

    str me::exprType::asImpli(const node& from, const type& to) const {
        str ret = ((node&) from).run() TO(asImpli(to)) OR.ret(str());
        return ret;
    }

    scope& me::subs() {
        return dumScope::singleton();
    }

    nbool me::isImpli(const type& to) const { return getEval()->isSub(to); }

    priorType me::prioritize(const args& a) const { return a.len() == 0 ? EXACT_MATCH : NO_MATCH; }

    const src& me::getSrc() const {
        WHEN(!_src).ret(super::getSrc());
        return *_src;
    }

    void me::_setSrc(const src& newSrc) { _src.bind(newSrc); }
} // namespace by
