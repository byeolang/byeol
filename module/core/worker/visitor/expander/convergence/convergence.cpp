#include "core/worker/visitor/expander/convergence/convergence.hpp"

namespace by {
    BY(DEF_ME(convergence))

    me::convergence(baseObj& obj, baseFunc& fun, onConverge closure): super(obj, fun, std::move(closure)) {}

    nbool me::convergeWithoutFrame() { return _getClosure()(); }
}
