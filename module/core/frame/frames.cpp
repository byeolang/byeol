#include "core/frame/frames.hpp"

#include "core/ast/baseFunc.hpp"
#include "core/frame/thread.hpp"

namespace by {
    BY(DEF_ME(frames))

    void me::dump() const {
        for(const auto& fr: *this)
            fr.dump();
    }
}
