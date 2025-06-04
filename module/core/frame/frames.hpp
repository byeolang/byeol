#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frame.hpp"

namespace nm {
    class _nout frames: public tnarr<frame>, public dumpable {
        NM(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
