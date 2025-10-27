#include "core/frame/frames.hpp"

#include "core/ast/baseFunc.hpp"
#include "core/frame/thread.hpp"

namespace by {
    BY(DEF_ME(frames))

    void me::dump() const {
        nidx n = 0;
        logger& log = logger::get();
        for(const auto& fr: *this) {
            tstr<nStr> funName = fr.getFunc() TO(template as<nStr>());
            log.logBypass("\tframe[" + std::to_string(n++) + "]: " + (funName ? funName->get() : "?") + "\n");
            fr.dump();
        }
    }
}
