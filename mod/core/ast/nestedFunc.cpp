#include "nestedFunc.hpp"
#include "../frame/frameInteract.hpp"

namespace nm {
    NM(DEF_ME(nestedFunc))

    me::nestedFunc(const func& org, const scope& subs):
        super(*new modifier(false, false), (mgdType&) org.getType(), subs, org.getBlock()) {}

    str me::_interactFrame(node& meObj, scope& s, nidx exN) {
        frameInteract f1(*this, s);
        {
            frameInteract f2(getBlock());
            { return _run(exN); }
        }
    }
}
