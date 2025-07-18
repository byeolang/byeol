#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frame.hpp"

namespace by {
    class _nout frames: public tnarr<frame>, public dumpable {
        BY(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
