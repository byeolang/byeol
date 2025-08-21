/// @file
#pragma once

#include "core/common.hpp"

namespace by {

    struct nerr;

    typedef nerr* (*errMaker)(nint);
    typedef void (*sigFunc)(const nerr&);
    typedef std::function<void(const nerr&)> sigHandler;

    /// @ingroup core
    /// @brief Signal structure for error handling
    /// @details Signal definition with error code and maker function.
    struct _nout sig {
        BY(ME(sig))

    public:
        nbool operator==(const sig& rhs) const;
        nbool operator!=(const sig& rhs) const;

    public:
        int code;
        errMaker maker;
    };

    /// @ingroup core
    /// @brief Signal handler manager
    /// @details Manages signal handlers and processes runtime signals and errors.
    class _nout signaler {
        BY(ME(signaler))

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
        const nerr* _getErrBy(nint code) const;

    private:
        std::vector<sigHandler> _closures;
    };
} // namespace by
