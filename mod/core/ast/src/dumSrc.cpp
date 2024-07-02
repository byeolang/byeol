#include "dumSrc.hpp"

namespace nm {
    NM(DEF_ME(dumSrc))

    namespace {
        static srcFile _inner("<no_name>", "");
    }

    dumSrc::dumSrc(): src(_inner, "", point {0, 0}) {}

    me& me::singletone() {
        static dumSrc inner;
        return inner;
    }
}
