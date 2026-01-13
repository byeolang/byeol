/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/frame/frame.hpp"

namespace by {
    /** @ingroup core
     *  @brief Container for execution frame stack
     *  @details Stack-based container managing execution frames for nested function calls. Since frames are closely
     *  related to the call stack, the frame created by the most recently called function comes first — in other words,
     *  it follows a LIFO (Last-In-First-Out) structure.
     *
     *  @section relationship_with_thread Relationship with thread
     *  Each @ref thread instance owns a frames object. When a function is called during program execution, a new @ref
     *  frame is pushed onto the frames stack. When the function returns, the frame is popped. This maintains the
     *  execution context for nested function calls and provides proper scoping for variables.
     *
     *  @section frame_lifecycle Frame Lifecycle
     *  Frames follow RAII principles - they are created when entering a function context and removed when exiting.
     *  However, as mentioned in @ref baseErr documentation, frames may continue to exist even after being removed from
     *  the stack if they are referenced by error objects (for callstack preservation).
     */
    class _nout frames: public tnarr<frame>, public dumpable {
        BY(CLASS(frames, tnarr<frame>))

    public:
        void dump() const override;
    };
}
