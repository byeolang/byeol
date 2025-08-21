/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frame.hpp"

namespace by {
    /// @ingroup core
    /// @brief Container for execution frame stack
    /// @details Stack-based container managing execution frames for nested function calls.
    class _nout frames: public tnarr<frame>, public dumpable {
        BY(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
