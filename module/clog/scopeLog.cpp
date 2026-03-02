#include "clog/scopeLog.hpp"
#include <chrono>

namespace by {
    BY(DEF_ME(scopeLog))
    
    me::scopeLog(std::function<void(nllong)> l): super([&]() {
        line::decLv();
        l(_measureNow() - _prev);
    }), _prev(_measureNow()) {}

    nllong me::_measureNow() {
        auto now = std::chrono::steady_clock::now();
        return duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    } 
}
