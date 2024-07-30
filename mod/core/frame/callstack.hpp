#pragma once

#include "../builtin/container/native/tnarr.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../frame/frames.hpp"
#include "../type/dumpable.hpp"

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

    typedef tnarr<calltrace> calltraces;

    class _nout callstack: public instance, public dumpable {
        NM(CLASS(callstack, instance))

    public:
        callstack();

    public:
        tucontainable<frame>::iter begin() const;
        tucontainable<frame>::iter end() const;

        const calltraces& getTraces() const;
        nbool hasTraces() const;
        void dump() const override;
        void setStack(const frames& frs);

    private:
        frames _stacks;
        mutable tstr<calltraces> _traces;
    };
} // namespace nm
