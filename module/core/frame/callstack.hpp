/// @file
#pragma once

#include "core/builtin/container/native/tnarr.hpp"
#include "core/builtin/container/native/tnbicontainer.hpp"
#include "core/frame/frames.hpp"

namespace by {

    class frame;
    class frames;

    /// @ingroup core
    /// @brief Single call trace entry
    /// @details Represents one entry in the call stack with frame and location information.
    class _nout calltrace: public instance {
        BY(CLASS(calltrace, instance))

    public:
        calltrace(const frame& newFrame);

    public:
        tstr<frame> fr;
        std::string at;
        std::string in;
    };

    /// @ingroup core
    /// @brief Call stack for error reporting and debugging
    /// @details Stack trace information for debugging and error reporting.
    class _nout callstack: public tnarr<calltrace>, public dumpable {
        BY(CLASS(callstack, instance))

    public:
        callstack();
        callstack(const frames& frs);

    public:
        void dump() const override;
    };
} // namespace by
