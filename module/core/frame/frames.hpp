/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frame.hpp"

namespace by {
    /// @ingroup core
    /// @brief Container for execution frame stack
    /// @details Stack-based container managing execution frames for nested function calls.
    /// Since frames are closely related to the call stack, the frame created by the
    /// most recently called function comes first — in other words, it follows a
    /// LIFO structure.
    class _nout frames: public tnarr<frame>, public dumpable {
        BY(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
