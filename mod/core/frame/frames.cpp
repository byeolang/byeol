#include "frames.hpp"

#include "../ast/baseFunc.hpp"
#include "thread.hpp"

namespace nm {
    NM(DEF_ME(frames))

    void me::dump() const {
        nidx n = 0;
        logger& log = logger::get();
        for(const auto& fr: thread::get().getFrames()) {
            log.logBypass("\tframe[" + std::to_string(n++) + "]:\n");
            fr.dump();
        }
    }
}
