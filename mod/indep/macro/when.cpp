#include "../common.hpp"
#include "../macro.hpp"

namespace nm {
    NM(DEF_ME(__dummy_class__))

    me::__dummy_class__() {}

    const me& me::get() {
        static me inner;
        return inner;
    }
}
