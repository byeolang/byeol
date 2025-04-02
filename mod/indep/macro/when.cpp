#include "../common.hpp"
#include "../macro.hpp"

namespace nm {
    NM(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }
}
