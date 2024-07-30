#pragma once

#include "../loader/err.hpp"

namespace nm {

    typedef err* (*errMaker)(nint);
    typedef void (*sigFunc)(const err&);
    typedef std::function<void(const err&)> sigHandler;

    struct _nout sig {
        NM(ME(sig))

    public:
        nbool operator==(const sig& rhs) const;
        nbool operator!=(const sig& rhs) const;

    public:
        int code;
        errMaker maker;
    };

    class _nout signaler {
        NM(ME(signaler))

    private:
        signaler();

    public:
        void addSignal(const sigHandler& closure);
        void delSignal(const sigHandler& closure);
        void relSignal();

        /// report that signal has occured. and terminate the process after reporting.
        void onSignal(nint code);

        static signaler& get();

    private:
        void _setSignal(void (*csignalHandler)(nint));
        const err* _getErrBy(nint code) const;

    private:
        std::vector<sigHandler> _closures;
    };
} // namespace nm
