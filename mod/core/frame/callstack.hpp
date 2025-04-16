#pragma once

#include "../builtin/container/native/tnarr.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../frame/frames.hpp"

namespace nm {

    class frame;
    class frames;

    class _nout calltrace: public instance {
        NM(CLASS(calltrace, instance))

    public:
        calltrace(const frame& newFrame);

    public:
        tstr<frame> fr;
        std::string at;
        std::string in;
    };

    class _nout callstack: public tnarr<calltrace>, public dumpable {
        NM(CLASS(callstack, instance))

    public:
        callstack();
        callstack(const frames& frs);

    public:
        void dump() const override;
    };
} // namespace nm
