#include "defaultPack.hpp"
#include "inputFunc.hpp"
#include "printFunc.hpp"
#include "../../primitive.hpp"
#include "../../../loader/err.hpp"
#include "../../../bridge/cpp/tcppBridge.hpp"
#include "../../../bridge/cpp/tcppBridgeFunc.inl"
#include "../../../loader/err.hpp"

namespace nm {
    NM(DEF_ME(defaultPack))

    scope& me::subs() {
        static scope inner;
        if(inner.len() <= 0) {
            inner.add("input", new inputFunc());
            inner.add("print", new printFunc<nStr>());
            inner.add("err", err::singletone());
        }
        return inner;
    }

    const baseObj& me::getOrigin() const {
        return *this;
    }
}
