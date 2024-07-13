#include "baseFunc.hpp"
#include "expr.hpp"
#include "../builtin/container/native/tnmap.inl"
#include "args.hpp"
#include "../frame/frame.hpp"
#include "dumScope.hpp"

namespace nm {

    NM_DEF_ME(expr)

    nbool me::exprType::isImpli(const type& to) const {
        return to.isSub<node>();
    }

    str me::exprType::asImpli(const node& from, const type& to) const {
        return str(((node&)from).run());
    }

    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    nbool me::isImpli(const type& to) const {
        return getEval()->isSub(to);
    }

    str me::asImpli(const type& to) const {
        me* unconst = (me*) this;
        str run = unconst->run();
        if(!run)
            return run;
        return run->asImpli(to);
    }

    priorType me::prioritize(const args& a) const {
        return a.len() == 0 ? EXACT_MATCH : NO_MATCH;
    }

    const src& me::getSrc() const {
        if(!_src) return super::getSrc();
        return *_src;
    }

    void me::_setSrc(const src& newSrc) {
        _src.bind(newSrc);
    }
}
