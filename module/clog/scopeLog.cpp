#include "clog/scopeLog.hpp"
#include <chrono>

namespace by {
    using namespace std::chrono;

    BY(DEF_ME(scopeLog))

    me::scopeLog(const std::function<void(nllong)>& l):
        super([l, this]() {
            line::decLv();
            l(_measureNow() - _prev);
        }),
        _prev(_measureNow()) {}

    nllong me::_measureNow() {
        auto now = steady_clock::now();
        return duration_cast<milliseconds>(now.time_since_epoch()).count();
    }
}
