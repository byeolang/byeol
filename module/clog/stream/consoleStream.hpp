/// @file
#pragma once

#include "clog/stream/stream.hpp"

namespace by {

    /** @ingroup clog
     *  @brief Console output stream for logging to standard output
     *  @details Implements stream interface to output log messages directly
     *  to the console/terminal with appropriate formatting.
     */
    class _nout consoleStream: public stream {
        BY_ME(consoleStream, stream)

    public:
        consoleStream();

    public:
        //  Stream:
        const std::string& getName() const override;

        using super::logBypass;
        nbool logBypass(const nchar* msg) override;
    };
}
