#include "core/ast/src/dumSrc.hpp"

namespace by {
    BY(DEF_ME(dumSrc))

    namespace {
        static srcFile _inner("<no_name>", "");
    }

    dumSrc::dumSrc(): src(_inner, "", point{0, 0}) {}

    void me::setFile(const srcFile& new1) {}
    void me::setPos(const point& new1) {}
    void me::setName(const std::string& new1) {}

    me& me::singleton() {
        static dumSrc inner;
        return inner;
    }
}
