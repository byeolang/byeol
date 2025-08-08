#include "core/ast/src/dumSrc.hpp"

namespace by {
    NM(DEF_ME(dumSrc))

    namespace {
        static srcFile _inner("<no_name>", "");
    }

    dumSrc::dumSrc(): src(_inner, "", point{0, 0}) {}

    me& me::singleton() {
        static dumSrc inner;
        return inner;
    }
}
