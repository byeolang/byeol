#include "core/ast/nestedFunc.hpp"
#include "core/frame/frameInteract.hpp"

namespace by {
    BY(DEF_ME(nestedFunc))

    me::nestedFunc(const func& org, const scope& subs):
        super(*new modifier(false, false), (funcType&) org.getType(), subs, org.getBlock()) {
        _setSrc(org.getSrc());
    }

    str me::_interactFrame(node& meObj, scope& s, nidx exN) {
        frameInteract f1(*this, s);
        frameInteract f2(getBlock());
        return _run(exN);
    }
}
