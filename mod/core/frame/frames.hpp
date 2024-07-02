#pragma once

#include "../type/dumpable.hpp"
#include "../builtin/container/native/tnarr.hpp"
#include "frame.hpp"

namespace nm {
    class _nout frames : public tnarr<frame>, public dumpable {
        NM(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
