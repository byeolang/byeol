#include "core/frame/frames.hpp"

#include "core/ast/baseFunc.hpp"
#include "core/frame/thread.hpp"

namespace by {
    BY(DEF_ME(frames))

    void me::dump() const {
        nidx n = 0;
        logger& log = logger::get();
        for(const auto& fr: *this) {
            log.logBypass("\tframe[" + std::to_string(n++) + "]:\n");
            fr.dump();
        }
    }
}